#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define SECTION_CHIEF_ALLOC_MAX_ACCOUNT (1000)
#define MINISTER_ALLOC_MAX_ACCOUNT ( 10000 )
typedef struct _Leader {
    struct _Leader *next;
    int account;
    int (*request)(struct _Leader *pLeader, int num);
}Leader;
void set_account(struct _Leader *pLeader, int account) 
{
    assert(NULL != pLeader);
    pLeader->account = account; 
}
void set_next_leader(struct _Leader *pLeader, struct _Leader *next)
{
    assert(NULL != pLeader && NULL != next);
    pLeader->next = next;
}
int request_for_manage(struct _Leader *pLeader, int num)
{
    assert(NULL != pLeader && 0 != num );
    if ( num < 10000)
        return 1;
    else if ( pLeader->next)
        return pLeader->next->request(pLeader->next, num);
    else return 0;
}
int request_for_boss(struct _Leader *pLeader, int num)
{
    assert(NULL != pLeader && 0 != num );
        return 1;
}
int main(int argc, char *argv[]) {
    if ( 2 != argc ){
        fprintf(stderr, "%s account",argv[0]);
        exit(EXIT_FAILURE);
    } //stack or used heap 
    Leader leader, leader1;
    memset (&leader, 0, sizeof(Leader));
    memset (&leader1, 0, sizeof(Leader));
    set_account(&leader, SECTION_CHIEF_ALLOC_MAX_ACCOUNT);
    set_account(&leader1, MINISTER_ALLOC_MAX_ACCOUNT);
    leader.next = &leader1;
    leader.request = request_for_manage;
    
    leader1.next = NULL;
    leader1.request = request_for_boss;
    int ret = -1;
    ret = leader.request(&leader, atoi(argv[1]));
    printf("ret:%d\n",ret );
    return 0;
}