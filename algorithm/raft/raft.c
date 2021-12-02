#include "raft.h"
/** raft周期性执行的函数,实现raft中的定时器以及定期应用日志到状态机
  */
int raft_periodic(raft_server_t* me_, int msec_since_last_period)
{
    raft_server_private_t* me = (raft_server_private_t*)me_;

    /* 选举计时器；Follower每次收到Leader的心跳后会重置清0，Leader每次发送日志也会清0 */
    me->timeout_elapsed += msec_since_last_period;

    /* Leader周期性地向Follower同步日志 */
    if (me->state == RAFT_STATE_LEADER) 
    {
        if (me->request_timeout <= me->timeout_elapsed)
            raft_send_appendentries_all(me_);
    }
    /* Follower检测选举计时器是否超时 */
    else if (me->election_timeout <= me->timeout_elapsed)
    {
        if (1 < me->num_nodes)
            raft_election_start(me_);
    }

    /* 周期性地将已经确认commit的日志应用到状态机FSM */
    if (me->last_applied_idx < me->commit_idx)
        if (-1 == raft_apply_entry(me_))
            return -1;

    return 0;
}

/** Follower成为Candidate执行的函数
  */
void raft_become_candidate(raft_server_t* me_)
{
    raft_server_private_t* me = (raft_server_private_t*)me_;
    int i;

    /*自增当前的任期号；给自己投票，设置自己的状态为CANDIDATE*/
    raft_set_current_term(me_, raft_get_current_term(me_) + 1);
    for (i = 0; i < me->num_nodes; i++)
        raft_node_vote_for_me(me->nodes[i], 0);
    raft_vote(me_, me->node);
    me->current_leader = NULL;
    raft_set_state(me_, RAFT_STATE_CANDIDATE);

    /* 重置选举超时计时器。为了防止多个Candidate竞争，将下一次发起投票的时间间隔设置成随机值*/
    /* TODO: this should probably be lower */
    me->timeout_elapsed = rand() % me->election_timeout;

    /*发送请求投票的 RPC 给其他所有服务器*/
    for (i = 0; i < me->num_nodes; i++)
        if (me->node != me->nodes[i] && raft_node_is_voting(me->nodes[i]))
            raft_send_requestvote(me_, me->nodes[i]);
}
/** 处理投票请求 
*/
int raft_recv_requestvote(raft_server_t* me_,
                          raft_node_t* node,
                          msg_requestvote_t* vr,
                          msg_requestvote_response_t *r)
{
    raft_server_private_t* me = (raft_server_private_t*)me_;
    
    /*如果请求中term > 本地currentTerm, 则转为Follower模式*/
    if (raft_get_current_term(me_) < vr->term)
    {
        raft_set_current_term(me_, vr->term);
        raft_become_follower(me_);
    }

    /*如果需要投票，则回复true，即将r->vote_granted = 1;*/
    if (__should_grant_vote(me, vr))
    {
        assert(!(raft_is_leader(me_) || raft_is_candidate(me_)));

        /*同意投票--本地记录给哪个服务器投了票，并设置response中的vote_granted为1*/
        raft_vote_for_nodeid(me_, vr->candidate_id);
        r->vote_granted = 1;

        /* there must be in an election. */
        me->current_leader = NULL;

        me->timeout_elapsed = 0;
    }
    else
        r->vote_granted = 0;

    __log(me_, node, "node requested vote: %d replying: %s",
          node, r->vote_granted == 1 ? "granted" : "not granted");

    /*更新本地保存的任期号，与请求中的保持一致*/
    r->term = raft_get_current_term(me_);
    return 0;
}

/** 检查是否满足投票的条件 
*/
static int __should_grant_vote(raft_server_private_t* me, msg_requestvote_t* vr)
{
    /**请求中的任期号term比本地term要小，不给投票*/
    if (vr->term < raft_get_current_term((void*)me))
        return 0;

    /*如果已经投过票了，返回false*/
    /* TODO: if voted for is candiate return 1 (if below checks pass) */
    if (raft_already_voted((void*)me))
        return 0;

    /* 下面代码检查请求中日志信息是否比本地日志新*/

    /* 获取本地最新的日志索引 */
    int current_idx = raft_get_current_idx((void*)me);

    /* 本地日志为空，请求中的日志信息绝对比本地要新，返回true */
    if (0 == current_idx)
        return 1;

    /* 如果本地最新日志中的任期号比请求中的last_log_term要小，则返回true */
    raft_entry_t* e = raft_get_entry_from_idx((void*)me, current_idx);
    if (e->term < vr->last_log_term)
        return 1;

    /* 本地最新日志中的任期号与请求中的last_log_term相等，则比较日志索引，索引比较大的说明日志比较新*/
    if (vr->last_log_term == e->term && current_idx <= vr->last_log_idx)
        return 1;

    /*果本地最新日志中的任期号比请求中的last_log_term要大，则返回false */
    return 0;
}
/** 处理投票恢复 
*/
int raft_recv_requestvote_response(raft_server_t* me_,
                                   raft_node_t* node,
                                   msg_requestvote_response_t* r)
{
    raft_server_private_t* me = (raft_server_private_t*)me_;

    __log(me_, node, "node responded to requestvote status: %s",
          r->vote_granted == 1 ? "granted" : "not granted");

    /* Oh~我不是Candidate，直接返回 */
    if (!raft_is_candidate(me_))
    {
        return 0;
    }
    /* response中的任期号比自己的大，说明自己的term已经过时，无条件转为Follower */
    else if (raft_get_current_term(me_) < r->term)
    {
        raft_set_current_term(me_, r->term);
        raft_become_follower(me_);
        return 0;
    }
    /* response中的任期号比自己小，说明收到了一个过时的response，忽略即可。
     * 当网络比较差的时候容易出现这种情况 */
    else if (raft_get_current_term(me_) != r->term)
    {
        return 0;
    }

    __log(me_, node, "node responded to requestvote: %d status: %s ct:%d rt:%d",
          node, r->vote_granted == 1 ? "granted" : "not granted",
          me->current_term,
          r->term);

    /* Yeah~给我投票了 */
    if (1 == r->vote_granted)
    {
        /* 记录给自己投票的服务器信息 */
        if (node)
            raft_node_vote_for_me(node, 1);
        int votes = raft_get_nvotes_for_me(me_);
        /* 如果给自己投票的服务器超过了总数的一般，则成为Leader */
        if (raft_votes_is_majority(me->num_nodes, votes))
            raft_become_leader(me_);
    }

    return 0;
}
/** 给某个Follower发送添加日志请求
  */
int raft_send_appendentries(raft_server_t* me_, raft_node_t* node)
{
    raft_server_private_t* me = (raft_server_private_t*)me_;

    assert(node);
    assert(node != me->node);

    /* callback函数，实现网络发送功能，由使用该raft实现的调用者实现网络IO功能*/
    if (!(me->cb.send_appendentries))
        return -1;

    /* 初始化请求的参数-- 当前任期号、最新日志索引 */
    msg_appendentries_t ae;
    ae.term = me->current_term;
    ae.leader_commit = raft_get_commit_idx(me_);
    ae.prev_log_idx = 0;
    ae.prev_log_term = 0;
    ae.n_entries = 0;
    ae.entries = NULL;

    /* 根据记录的Follower的日志信息，获取要发给Follower的下一条日志索引 */
    int next_idx = raft_node_get_next_idx(node);

    msg_entry_t mety;

    /* 添加下一条日志的内容*/
    raft_entry_t* ety = raft_get_entry_from_idx(me_, next_idx);
    if (ety)
    {
        mety.term = ety->term;
        mety.id = ety->id;
        mety.type = ety->type;
        mety.data.len = ety->data.len;
        mety.data.buf = ety->data.buf;
        ae.entries = &mety;
        // TODO: we want to send more than 1 at a time
        ae.n_entries = 1;
    }

    /* 添加要添加日志的前一条日志信息，用来做日志一致性检查，关于怎么保证
     * Leader和Follower日志的一致性，可参看raft论文第5.3节--日志复制*/
    if (1 < next_idx)
    {
        raft_entry_t* prev_ety = raft_get_entry_from_idx(me_, next_idx - 1);
        ae.prev_log_idx = next_idx - 1;
        if (prev_ety)
            ae.prev_log_term = prev_ety->term;
    }

    __log(me_, node, "sending appendentries node: ci:%d t:%d lc:%d pli:%d plt:%d",
          raft_get_current_idx(me_),
          ae.term,
          ae.leader_commit,
          ae.prev_log_idx,
          ae.prev_log_term);

    /* 调用callback发送请求，callback由该raft实现的调用者来实现*/
    me->cb.send_appendentries(me_, me->udata, node, &ae);

    return 0;
}
/*
1. 处理任期号的三种情况(大于等于和小于)
2. 处理prev log不一致的情况，返回包中告诉Leader自己目前的log情况
3. 处理添加日志成功的情况-- 保存新日志并更新current_idx和commit_idx
*/
int raft_recv_appendentries(
    raft_server_t* me_,
    raft_node_t* node,
    msg_appendentries_t* ae,
    msg_appendentries_response_t *r
    )
{
    raft_server_private_t* me = (raft_server_private_t*)me_;

    me->timeout_elapsed = 0;

    if (0 < ae->n_entries)
        __log(me_, node, "recvd appendentries from: %lx, t:%d ci:%d lc:%d pli:%d plt:%d #%d",
              node,
              ae->term,
              raft_get_current_idx(me_),
              ae->leader_commit,
              ae->prev_log_idx,
              ae->prev_log_term,
              ae->n_entries);

    r->term = me->current_term;

    /* 处理任期号 */
    /* currentTerm == ae->term,当自己是Candidate时收到term与自己相等的请求，
     * 说明已经有其它Candidate成为了Leader,自己无条件变成Follower*/
    if (raft_is_candidate(me_) && me->current_term == ae->term)
    {
        me->voted_for = -1;
        raft_become_follower(me_);
    }
    /* currentTerm < ae->term. 自己的任期号已经落后Leader，无条件成为Follower，并且更新自己的term*/
    else if (me->current_term < ae->term)
    {
        raft_set_current_term(me_, ae->term);
        r->term = ae->term;
        raft_become_follower(me_);
    }
    /* currentTerm > ae->term. 说明收到一个过时Leader的请求，直接回包告诉它最新的term */
    else if (ae->term < me->current_term)
    {
        /* 1. Reply false if term < currentTerm (Â§5.1) */
        __log(me_, node, "AE term %d is less than current term %d",
              ae->term, me->current_term);
        goto fail_with_current_idx;
    }

    
    /* NOTE: the log starts at 1 */
    /* 检查请求中prev_log_idx日志的term与本地对应索引的term是否一致 */
    if (0 < ae->prev_log_idx)
    {
        raft_entry_t* e = raft_get_entry_from_idx(me_, ae->prev_log_idx);

        /*  本地在prev_log_idx位置还不存在日志，说明日志已经落后Leader了，返回false
         *    并告诉leader自己当前日志的位置，这样Leader知道下一次该发哪条日志过来了*/
        if (!e)
        {
            __log(me_, node, "AE no log at prev_idx %d", ae->prev_log_idx);
            goto fail_with_current_idx;
        }
        if (raft_get_current_idx(me_) < ae->prev_log_idx)
            goto fail_with_current_idx;

        /* 本地在prev_log_idx位置的日志的term与请求中的prev_log_term不一致，
         * 此时本地无条件删除本地与请求不一致的日志，并向Leader返回删除后的日志位置*/
        if (e->term != ae->prev_log_term)
        {
            __log(me_, node, "AE term doesn't match prev_term (ie. %d vs %d) ci:%d pli:%d",
                  e->term, ae->prev_log_term, raft_get_current_idx(me_), ae->prev_log_idx);
            assert(me->commit_idx < ae->prev_log_idx);
            /* Delete all the following log entries because they don't match */
            log_delete(me->log, ae->prev_log_idx);
            r->current_idx = ae->prev_log_idx - 1;
            goto fail;
        }
    }

    /* 本地的日志比Leader要多。当本地服务器曾经是Leader，收到了很多客户端请求
     * 并还没来得及同步时会出现这种情况。这时本地无条件删除比Leader多的日志 */
    if (ae->n_entries == 0 && 0 < ae->prev_log_idx && ae->prev_log_idx + 1 < raft_get_current_idx(me_))
    {
        assert(me->commit_idx < ae->prev_log_idx + 1);
        log_delete(me->log, ae->prev_log_idx + 1);
    }

    r->current_idx = ae->prev_log_idx;

    /* 下面for循环跳过请求中已经在本地添加过的日志*/
    int i;
    for (i = 0; i < ae->n_entries; i++)
    {
        msg_entry_t* ety = &ae->entries[i];
        int ety_index = ae->prev_log_idx + 1 + i;
        raft_entry_t* existing_ety = raft_get_entry_from_idx(me_, ety_index);
        r->current_idx = ety_index;
        if (existing_ety && existing_ety->term != ety->term)
        {
            assert(me->commit_idx < ety_index);
            log_delete(me->log, ety_index);
            break;
        }
        else if (!existing_ety)
            break;
    }

    /* 下面for循环将请求中确认的新日志添加到本地 */
    for (; i < ae->n_entries; i++)
    {
        int e = raft_append_entry(me_, &ae->entries[i]);
        if (-1 == e)
            goto fail_with_current_idx;

        r->current_idx = ae->prev_log_idx + 1 + i;
    }

    /* 4. 请求中携带了Leader已经提交到状态机的日志索引，本地同样也更新这个索引，将其
     *    设置为本地最大日志索引和leader_commit中的较小者*/
    if (raft_get_commit_idx(me_) < ae->leader_commit)
    {
        int last_log_idx = max(raft_get_current_idx(me_), 1);
        raft_set_commit_idx(me_, min(last_log_idx, ae->leader_commit));
    }

    /* 更新Leader信息 */
    me->current_leader = node;

    r->success = 1;
    r->first_idx = ae->prev_log_idx + 1;
    return 0;

fail_with_current_idx:
    r->current_idx = raft_get_current_idx(me_);
fail:
    r->success = 0;
    r->first_idx = 0;
    return -1;
}

/** 处理添加日志请求回复
  */
int raft_recv_appendentries_response(raft_server_t* me_,
                                     raft_node_t* node,
                                     msg_appendentries_response_t* r)
{
    raft_server_private_t* me = (raft_server_private_t*)me_;

    __log(me_, node,
          "received appendentries response %s ci:%d rci:%d 1stidx:%d",
          r->success == 1 ? "SUCCESS" : "fail",
          raft_get_current_idx(me_),
          r->current_idx,
          r->first_idx);

    /* 过时的回复 -- 忽略 */
    if (r->current_idx != 0 && r->current_idx <= raft_node_get_match_idx(node))
        return 0;

    /* oh~我不是Leader */
    if (!raft_is_leader(me_))
        return -1;

    /* 回复中的term比自己的要大，说明自己是一个过时的Leader，无条件转为Follower */
    if (me->current_term < r->term)
    {
        raft_set_current_term(me_, r->term);
        raft_become_follower(me_);
        return 0;
    }
    /* 过时的回复，网络状况不好时会出现 */
    else if (me->current_term != r->term)
        return 0;

    /* stop processing, this is a node we don't have in our configuration */
    if (!node)
        return 0;

    /* 由于日志不一致导致添加日志不成功*/
    if (0 == r->success)
    {
        assert(0 <= raft_node_get_next_idx(node));

        /* 将nextIdex减*/
        int next_idx = raft_node_get_next_idx(node);
        assert(0 <= next_idx);
        /* Follower的日志数量还远远少于Leader，将nextIdex设为回复中的current_idx+1和Leader
         * 当前索引中较小的一个，一般回复中的current_idx+1会比较小*/
        if (r->current_idx < next_idx - 1)
            raft_node_set_next_idx(node, min(r->current_idx + 1, raft_get_current_idx(me_)));
        /* Follower的日志数量和Leader差不多，但是比对前一条日志时失败，这种情况将next_idx减1
         * 重试*/
        else
            raft_node_set_next_idx(node, next_idx - 1);

        /* 使用更新后的nextIdx重新发送添加日志请求 */
        raft_send_appendentries(me_, node);
        return 0;
    }

    assert(r->current_idx <= raft_get_current_idx(me_));

    /* 下面处理添加日志请求的情况 */
    /* 更新本地记录的Follower的日志情况 */
    raft_node_set_next_idx(node, r->current_idx + 1);
    raft_node_set_match_idx(node, r->current_idx);

    /* 如果是新加入的机器，则判断它的日志是否是最新，如果达到了最新，则赋予它投票权，
     * 这里逻辑的详细解释在第3.4节 -- 成员变更*/
    if (!raft_node_is_voting(node) &&
        -1 == me->voting_cfg_change_log_idx &&
        raft_get_current_idx(me_) <= r->current_idx + 1 &&
        me->cb.node_has_sufficient_logs &&
        0 == raft_node_has_sufficient_logs(node)
        )
    {
        raft_node_set_has_sufficient_logs(node);
        me->cb.node_has_sufficient_logs(me_, me->udata, node);
    }

    /* 如果一条日志回复成功的数量超过一半，则将日志提交commit，即允许应用到状态机 */
    int votes = 1; /* include me */
    int point = r->current_idx;
    int i;
    for (i = 0; i < me->num_nodes; i++)
    {
        if (me->node == me->nodes[i] || !raft_node_is_voting(me->nodes[i]))
            continue;

        int match_idx = raft_node_get_match_idx(me->nodes[i]);

        if (0 < match_idx)
        {
            raft_entry_t* ety = raft_get_entry_from_idx(me_, match_idx);
            /*如果follower已经添加了索引大于等于r->current_idx的日志，则vote加1*/
            if (ety->term == me->current_term && point <= match_idx)
                votes++;
        }
    }

    /* 投票数大于所有服务器的一半，则将日志提交 */
    if (me->num_nodes / 2 < votes && raft_get_commit_idx(me_) < point)
        raft_set_commit_idx(me_, point);

    /* 如果follower的日志还没有最新，那么继续发送添加日志请求 */
    if (raft_get_entry_from_idx(me_, raft_node_get_next_idx(node)))
        raft_send_appendentries(me_, node);

    /* periodic applies committed entries lazily */

    return 0;
}