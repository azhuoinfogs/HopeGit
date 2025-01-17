
// ================================================
// Common Log Module - logrotate based edition
// ================================================

#include <stdlib.h>
#include <sys/file.h>
#include "log.h"
#include <map>

CLog::CLog()
{
    pthread_mutex_init(&m_mutex, NULL);
    pthread_mutex_init(&m_flush_mutex, NULL);

    m_lastFlushTime = 0;
    m_warnItemNum = 0;
    m_critItemNum = 0;
    m_thread_is_create = false;
    m_exit = false;
    m_fork_process = false;
    m_log_fd = -1;
    m_thread_id = 0;
    m_screen_stream = stderr;

    SLogContext ctx_default;
    ctx_default.log_level = LOG_PRI_WARN;
    ctx_default.log_color = LOG_CLR_PID;
    ctx_default.b_log_screen = true;
    ctx_default.screen_stream = stderr;
    ctx_default.b_log_rotate = false;
    strcpy(ctx_default.log_file_name, "");
}

CLog::~CLog()
{
    _flushLogQueue(true);
    pthread_mutex_lock(&m_flush_mutex);
    m_exit = true;
    if(m_log_fd != -1) {
        close(m_log_fd);
        m_log_fd = -1;
    }
    pthread_mutex_unlock(&m_flush_mutex);

    if(m_thread_is_create) {
        pthread_join(m_thread_id, NULL);
    }
    pthread_mutex_destroy(&m_mutex);
    pthread_mutex_destroy(&m_flush_mutex);
}

void CLog::setContext(const SLogContext* ctx)
{
    if((!m_ctx.b_log_screen && ctx->b_log_screen) || (strcmp(ctx->log_file_name, m_ctx.log_file_name) != 0)) {
        _flushLogQueue(true);
        pthread_mutex_lock(&m_flush_mutex);
        if(m_log_fd != -1) {
            close(m_log_fd);
            m_log_fd = -1;
        }
        pthread_mutex_unlock(&m_flush_mutex);
    }

    memcpy(&m_ctx, ctx, sizeof(m_ctx));
    m_screen_stream = ctx->screen_stream;
    m_lastFlushTime = 0;
    m_warnItemNum = 0;
    m_critItemNum = 0;
}

int CLog::convStrToLoglevel(const char* str_log_level, int defaultLogLevel)
{
    if(!str_log_level || *str_log_level == '\0') {
        return defaultLogLevel;
    }
    struct ltStrNocase {
        bool operator()(const char* s1, const char* s2) const
        {
            return strcasecmp(s1, s2) < 0;
        }
    };
    static const std::map<const char*, int, ltStrNocase> sLogLevelMap = {
        {"critical", LOG_PRI_CRITICAL},
        {"warn",     LOG_PRI_WARN},
        {"info",     LOG_PRI_INFO},
        {"debug",    LOG_PRI_DEBUG}
    };
    auto it = sLogLevelMap.find(str_log_level);
    if(it != sLogLevelMap.end()) {
        return it->second;
    }
    return defaultLogLevel;
}

void CLog::setLogLevel(const int log_level)
{
    m_ctx.log_level = log_level;
}

static int64_t LogRotateSize()
{
    char c;
    int64_t  num;
    int64_t    ret;
    int64_t     mult;
    char buf[32];

    strcpy(buf, LOGROTATE_ROTATE_SIZE);
    c = buf[strlen(buf) - 1];
    if(c > '9') {
        buf[strlen(buf) - 1] = '\0';
    }
    double f = atof(buf);
    num = atoll(buf);

    if(isdigit(c))
        return num;

    switch(c) {
    case 't':
    case 'T':
        mult = 0x1ULL << 40; //1024 * 1024 * 1024*1024;
        break;
    case 'g':
    case 'G':
        mult = 0x1ULL << 30; //1024 * 1024 * 1024;
        break;
    case 'm':
    case 'M':
        mult = 0x1ULL << 20;//1024 * 1024;
        break;
    case 'k':
    case 'K':
        mult = 0x1ULL << 10;//1024;
        break;
    case 'b':
    case 'B':
        mult = 1;
        break;
    default:
        return -1;
    }

    ret = mult * f;
    return ret;
}

void CLog::_configLogrotate(int fd)
{
    // NOTE: Call it ONLY inside an external mutex lock
    static const int64_t rotate_size = LogRotateSize();
    int flen = strlen(m_ctx.log_file_name);
    if(flen == 0 || m_ctx.log_file_name[0] != '/')
        return;
    const char *p_slash = strrchr(m_ctx.log_file_name, '/');
    if(p_slash == m_ctx.log_file_name + flen - 1)
        return;

    char config_file[PATH_MAX + 1];
    config_file[PATH_MAX] = 0;
    snprintf(config_file, PATH_MAX, "%s/%s.conf", LOGROTATE_CONFIG_PATH, p_slash + 1);

    if(!m_ctx.b_log_rotate)
        unlink(config_file);
    else {
        bool need_rotate = false;
        int ret = access(config_file, F_OK);
        if(ret < 0) {
            //logrotate文件不能使用666权限, 否则在执行logrotate时会报如下错误：
            //error: Ignoring /etc/logrotate.d//samba_bfs_adaptor.log.conf because it is writable by group or others.
            int cfd = open(config_file, O_WRONLY|O_TRUNC|O_CREAT, 0644);
            if(cfd < 0) {
                return;
            }
            //消除编译告警: directive output may be truncated writing 68 bytes into a region of size between 0 and 4096 [-Wformat-truncation=]
            //              note: ‘snprintf’ output between 83 and 4179 bytes into a destination of size 4096
            static const int bufLen = PATH_MAX+1024;
            char buf[bufLen+1];
            buf[bufLen] = 0;
            snprintf(buf, bufLen, "%s\n{\nweekly\nmissingok\nnotifempty\ncompress\ncopytruncate\nstart 1\n"
                    "rotate %d\nsize %s\n}", m_ctx.log_file_name, LOGROTATE_ROTATE_NUM, LOGROTATE_ROTATE_SIZE);
            write(cfd, buf, strlen(buf));
            close(cfd);
        }
        // start logrotate
        if(!need_rotate) {
            struct stat log_file_stat;
            ret = fstat(fd, &log_file_stat);
            if(!ret && log_file_stat.st_size >= rotate_size) {
                need_rotate = true;
            }
        }
        if(need_rotate) {
            int lockFd = -1;
            bool bLockSucc = false;
            if(m_fork_process) {
                //多进程在执行logrotate时加文件锁
                char lockFilePath[PATH_MAX + 1];
                snprintf(lockFilePath, PATH_MAX, "/tmp/%s.lock", p_slash + 1);
                mode_t oldmask = umask(0);
                lockFd = open(lockFilePath, O_RDWR | O_CREAT, 0666);
                umask(oldmask);
                if(lockFd > 0) {
                    int ret = flock(lockFd, LOCK_EX);//lock file
                    if(0 == ret) {
                        bLockSucc = true;
                        struct stat log_file_stat;
                        ret = stat(m_ctx.log_file_name, &log_file_stat);
                        //说明文件已经被logrotate, 直接返回, 不执行logrotate
                        if(ret == 0 && log_file_stat.st_size < rotate_size) {
                            flock(lockFd, LOCK_UN);
                            close(lockFd);
                            return;
                        }
                    }
                }
            }
            static const int cmdBufLen = 1023;
            char cmd_buf[cmdBufLen+1];
            cmd_buf[cmdBufLen] = 0;
            snprintf(cmd_buf, cmdBufLen, "logrotate -f %s", config_file);
            system(cmd_buf);

            if(lockFd > 0) {
                if(bLockSucc) {
                    flock(lockFd, LOCK_UN);
                }
                close(lockFd);
            }
        }
    }
}

void CLog::_formatLogItem(char* buf,
                          const int log_priority,
                          const char* log_format,
                          va_list* vargs,
                          bool b_log_screen
                         )
{
    pid_t pid = _gettid();

    long long ts = _getTimestamp(true);
    long long tsec = ts / 1000000;
    long long tusec = ts - tsec * 1000000;

    char log_type = 'W';
    switch(log_priority) {
    case LOG_PRI_WARN:
        log_type = 'W';
        break;
    case LOG_PRI_INFO:
        log_type = 'I';
        break;
    case LOG_PRI_DEBUG:
        log_type = 'D';
        break;
    case LOG_PRI_CRITICAL:
        log_type = 'C';
        break;
    }

    if(b_log_screen) {
        char buf_dtt[32];
        time_t tt = (time_t)tsec;
        tm *tmt = localtime(&tt);
        strftime(buf_dtt, sizeof(buf_dtt), "%H:%M:%S", tmt);
        snprintf(buf, 28, "[%-6d %s.%06lld %c] ", pid, buf_dtt,tusec,log_type);
        vsnprintf(buf + 27, MAX_LOG_ITEM_LENGTH - 27, log_format, *vargs);
    } else {
        char buf_dtt[32];
        time_t tt = (time_t)tsec;
        tm *tmt = localtime(&tt);
        strftime(buf_dtt, sizeof(buf_dtt), "20%y%m%d-%H:%M:%S", tmt);

        memset(buf, 0, MAX_LOG_ITEM_LENGTH + 1);
        snprintf(buf, 37, "[%-6d %s.%06lld %c] ", pid, buf_dtt, tusec, log_type);
        vsnprintf(buf + 36, MAX_LOG_ITEM_LENGTH - 36 - 1, log_format, *vargs);
        int slen = strlen(buf);
        while(slen > 0 && '\n' == buf[slen-1]) {
            --slen;
        }
        buf[slen] = '\n';
        buf[slen + 1] = 0;
    }
}

void CLog::_printLogItem(const char* buf, const int log_highlight)
{
    if(!m_screen_stream) {
        fprintf(stderr, "m_screen_stream is null\n");
        return;
    }
    int log_color = _getLogColor();

    if(log_highlight == LOG_HL_KEYT)
        fprintf(m_screen_stream, "\033[1;37;41m%s\033[0m\n", buf);
    else if(log_highlight == LOG_HL_MARK) {
        switch(log_color) {
        case LOG_CLR_GRAY:
            fprintf(m_screen_stream, "\033[30;47m%s\033[0m\n", buf);
            break;
        case LOG_CLR_RED:
        case LOG_CLR_ORANGE:
        case LOG_CLR_PURPLE:
        case LOG_CLR_LPURPLE:
            fprintf(m_screen_stream, "\033[30;45m%s\033[0m\n", buf);
            break;
        case LOG_CLR_YELLOW:
            fprintf(m_screen_stream, "\033[30;43m%s\033[0m\n", buf);
            break;
        case LOG_CLR_BLUE:
            fprintf(m_screen_stream, "\033[30;44m%s\033[0m\n", buf);
            break;
        case LOG_CLR_CYAN:
        case LOG_CLR_LCYAN:
            fprintf(m_screen_stream, "\033[30;46m%s\033[0m\n", buf);
            break;
        case LOG_CLR_GREEN:
        case LOG_CLR_LGREEN:
        default:
            fprintf(m_screen_stream, "\033[30;42m%s\033[0m\n", buf);
            break;
        }
    } else {
        if(log_color < 0)
            fprintf(m_screen_stream, "\033[1;%dm%s\033[0m\n", -1 * log_color, buf);
        else
            fprintf(m_screen_stream, "\033[%dm%s\033[0m\n", log_color, buf);
    }
}

void CLog::_flushLogImp(int fd)
{
    // NOTE: Call it ONLY inside an external mutex lock
    if(m_flush_cache.empty())
        return;

    int count = 0;
    for(auto &str : m_flush_cache) {
        if(count < MAX_LOG_QUEUE_LENGTH) {
            m_iovs[count].iov_base = (void*)str.c_str();
            m_iovs[count].iov_len = str.size();
            ++count;
        } else {
            writev(fd, m_iovs, count);
            count = 0;
            m_iovs[count].iov_base = (void*)str.c_str();
            m_iovs[count].iov_len = str.size();
            ++count;
        }
    }
    if(count > 0) {
        writev(fd, m_iovs, count);
        _configLogrotate(fd);
    }
    m_flush_cache.clear();
}


void CLog::_flushLogInNewThreadWrapper()
{
    while(!m_exit) {
        pthread_mutex_lock(&m_flush_mutex);
        if(!m_flush_cache.empty()) {
            if(-1 == m_log_fd) {
                //为兼容非root身份进程, 比如samba进程
                mode_t oldmask = umask(0);
                m_log_fd = open(m_ctx.log_file_name, O_WRONLY | O_APPEND | O_CREAT, 0666);
                umask(oldmask);
                if(-1 == m_log_fd) {
                    pthread_mutex_unlock(&m_flush_mutex);
                    usleep(3000000); //sleep 3 seconds, then retry
                    continue;
                }
            }
            _flushLogImp(m_log_fd);
            pthread_mutex_unlock(&m_flush_mutex);

            m_warnItemNum = 0;
            m_critItemNum = 0;
            m_lastFlushTime = _getTimestamp();
            continue;
        }
        pthread_mutex_unlock(&m_flush_mutex);
        usleep(500000);//sleep 500ms

        //进入此分支m_flush_cache肯定为空，以防万一，再加一次判断
        pthread_mutex_lock(&m_flush_mutex);
        if(!m_flush_cache.empty()) {
            pthread_mutex_unlock(&m_flush_mutex);
            continue;
        }
        pthread_mutex_unlock(&m_flush_mutex);

        //至此，m_flush_cache为空，可以安全的和m_work_cache进行交换
        long long time_span = _getTimestamp() - m_lastFlushTime;
        pthread_mutex_lock(&m_mutex);
        if(m_work_cache.size() >= MAX_LOG_QUEUE_LENGTH
           || (!m_work_cache.empty() && (time_span >= MAX_LOG_QUEUE_TIME))) {
            pthread_mutex_lock(&m_flush_mutex);
            std::swap(m_work_cache, m_flush_cache);
            pthread_mutex_unlock(&m_flush_mutex);
        }
        pthread_mutex_unlock(&m_mutex);
    }
}

void* CLog::_flushLogInNewThread(void* arg)
{
    ((CLog*)arg)->_flushLogInNewThreadWrapper();
    return NULL;
}

void CLog::_flushLogQueue(bool force)
{
    pthread_mutex_lock(&m_flush_mutex);
    if(m_log_fd == -1) {
        //为兼容非root身份进程, 比如samba进程
        mode_t oldmask = umask(0);
        m_log_fd = open(m_ctx.log_file_name, O_WRONLY | O_APPEND | O_CREAT, 0666);
        umask(oldmask);
    }
    pthread_mutex_unlock(&m_flush_mutex);

    pthread_mutex_lock(&m_mutex);
    if(!m_work_cache.empty()) {
        pthread_mutex_lock(&m_flush_mutex);
        if(m_log_fd != -1) {
            _flushLogImp(m_log_fd);
            std::swap(m_work_cache, m_flush_cache);
        }
        pthread_mutex_unlock(&m_flush_mutex);
    }
    pthread_mutex_unlock(&m_mutex);

    pthread_mutex_lock(&m_flush_mutex);
    if(m_log_fd != -1) {
        _flushLogImp(m_log_fd);
    }
    pthread_mutex_unlock(&m_flush_mutex);
}

void CLog::_log(const int log_priority, const int log_highlight, const char* log_format, va_list* vargs)
{
    if(log_priority > m_ctx.log_level)
        return;
    bool bLogScreen = m_ctx.b_log_screen;
    char buf[MAX_LOG_ITEM_LENGTH + 1];
    _formatLogItem(buf, log_priority, log_format, vargs, bLogScreen);

    if(bLogScreen) {
        _printLogItem(buf, log_highlight);
    } else {
        pthread_mutex_lock(&m_mutex);
        if(!m_thread_is_create) {
            int ret = pthread_create(&m_thread_id, NULL, _flushLogInNewThread, (void*)this);
            if(!ret) {
                m_thread_is_create = true;
                pthread_setname_np(m_thread_id, "boss_log_thread");
            }
        }
        m_work_cache.push_back(buf);
        pthread_mutex_unlock(&m_mutex);
    }
}

void CLog::ReinitInForkChildProcess()
{
    //fork进程时，如果刚好锁被占用，在子进程中clone过来的锁也是锁住状态，所以需要处理
    //这里使用trylock，如果锁在父进程被锁住，会返回busy，不会阻塞，然后可以在子进程安全释放
    //如果在父进程没被锁住，在子进程中获取锁权限后，立马被释放，达到清洗锁状态目的
    pthread_mutex_trylock(&m_mutex);
    pthread_mutex_unlock(&m_mutex);

    pthread_mutex_trylock(&m_flush_mutex);
    pthread_mutex_unlock(&m_flush_mutex);

    //fork进程时，因为线程不会被clone过来，但是m_thread_is_create会被复制过来
    //从而导致在子进程中不能创建新Log线程
    m_thread_is_create = false;
    m_thread_id = 0;
    m_fork_process = true;
}

