
// ================================================
// Common Log Module - logrotate based edition
// ================================================

#ifndef __LOG_H__
#define __LOG_H__

#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <ctime>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <sys/syscall.h>
#include <pthread.h>
#include <vector>
#include <string>
#include <atomic>
#include <list>

using std::vector;
using std::string;
using std::list;

#if 0
    #define BossDEBUG  printf
    #define BossINFO   printf
    #define BossWARN   printf
    #define BossCRIT   printf

    #define BossDEBUG CLog::getInstance().DEBUG
    #define BossINFO CLog::getInstance().INFO
    #define BossWARN CLog::getInstance().WARN
    #define BossCRIT CLog::getInstance().CRIT
#endif

// log priority and external control
#define LOG_PRI_CRITICAL               10
#define LOG_PRI_WARN                   20
#define LOG_PRI_INFO                   30
#define LOG_PRI_DEBUG                  40


extern unsigned int gLogLevel;
static inline int LogCritical(unsigned int logLevel)
{
    return logLevel >= LOG_PRI_CRITICAL;
}
static inline int LogWarn(unsigned int logLevel)
{
    return logLevel >= LOG_PRI_WARN;
}
static inline int LogInfo(unsigned int logLevel)
{
    return logLevel >= LOG_PRI_INFO;
}
static inline int LogDebug(unsigned int logLevel)
{
    return logLevel >= LOG_PRI_DEBUG;
}
void LogPrint(int, const char*, ...);

// log highlight option
#define LOG_HL_NONE                    0
#define LOG_HL_MARK                    1             // mark the log item with the same color background
#define LOG_HL_KEYT                    2             // mark the log item with red background and white text color

// log color option
#define LOG_CLR_NONE                   0             // use default log color
#define LOG_CLR_PID                    1             // use process-related log color
#define LOG_CLR_TID                    2             // use thread-related log color
#define LOG_CLR_GRAY                   -30
#define LOG_CLR_RED                    31
#define LOG_CLR_ORANGE                 -31
#define LOG_CLR_GREEN                  32
#define LOG_CLR_LGREEN                 -32
#define LOG_CLR_YELLOW                 -33
#define LOG_CLR_BLUE                   -34
#define LOG_CLR_PURPLE                 35
#define LOG_CLR_LPURPLE                -35
#define LOG_CLR_CYAN                   36
#define LOG_CLR_LCYAN                  -36

#define MAX_LOG_ITEM_LENGTH            1023
#define MAX_LOG_QUEUE_LENGTH           1024
#define MAX_LOG_QUEUE_TIME             1  // 1 second

#define LOGROTATE_CONFIG_PATH          "/etc/logrotate.d/"
#define LOGROTATE_ROTATE_SIZE          "100M"
#define LOGROTATE_ROTATE_NUM           50

class CLog
{
public:
    // Structure of log configuration context
    struct SLogContext {
        // The log level. Only those log items whose priority is less than
        // or equals to it will be printed to screen or written to the disk.
        int log_level;// = LOG_PRI_WARN;

        // The log color control code of current thread. If it's LOG_CLR_NONE,
        // we will use default text color, and, if it's LOG_CLR_PID or LOG_CLR_TID,
        // we will use specific text color colordepending on the pid or tid.
        // Otherwise, the specified text color will be used.
        int log_color;// = LOG_CLR_PID;

        // Whether to print log items to screen or not. If true, the subsequent
        // log items will neither be pushed into the log queue nor written to the
        // log files, until this is set to false.
        bool b_log_screen;// = true;

        //standard I/O streams : stdin, stdout, stderr
        FILE* screen_stream;// = stderr

        // Whether to configure and use logrotate
        bool b_log_rotate;// = false;

        // The full path of the log file.
        char log_file_name[PATH_MAX + 1];

        SLogContext()
            : log_level(LOG_PRI_WARN),
              log_color(LOG_CLR_PID),
              b_log_screen(true),
              screen_stream(stderr)
        {}
    };

    // ------------------------------------------------
    // Get the singleton instance of CLog
    // ------------------------------------------------
    static CLog& getInstance()
    {
        // NOTE: Only since C++11 standard is the thread safe of static local variable ensured.
        static CLog instance;
        return instance;
    }

    //convert {critical, warn, info, debug } to {LOG_PRI_CRITICAL, LOG_PRI_WARN, LOG_PRI_INFO, LOG_PRI_DEBUG}
    //if str_log_level not in {critical, warn, info, debug }, defaultLogLevel will be returned
    static int convStrToLoglevel(const char* str_log_level, int defaultLogLevel = LOG_PRI_WARN);

    // ------------------------------------------------
    // Configure the log context
    // ------------------------------------------------
    void setContext(const SLogContext* ctx);

    // ------------------------------------------------
    // Change the log level option
    // ------------------------------------------------
    void setLogLevel(const int log_level);

    inline int getLogLevel() const
    {
        return m_ctx.log_level;
    }

    // ------------------------------------------------
    // Get Log file fd
    // ------------------------------------------------
    int getLogFd()
    {
        return m_log_fd;
    }

    // ------------------------------------------------
    // Log a new item
    // ------------------------------------------------
    void LOG(const int log_priority, const int log_highlight, const char* log_format, ...) __attribute__((format(printf,4,5)))
    {
        va_list vargs;
        va_start(vargs, log_format);
        _log(log_priority, log_highlight, log_format, &vargs);
        va_end(vargs);
    }

    // ------------------------------------------------
    // Log a new CRITICAL item
    // ------------------------------------------------
    void CRITICAL(const char* log_format, ...) __attribute__((format(printf,2,3)))
    {
        // filter overloaded log items
        if((time(0) - m_lastFlushTime == 0) && (m_critItemNum > 20))
            return;
        ++ m_critItemNum;

        va_list vargs;
        va_start(vargs, log_format);
        _log(LOG_PRI_CRITICAL, LOG_HL_NONE, log_format, &vargs);
        va_end(vargs);
    }

    // ------------------------------------------------
    // Log a new WARN item
    // ------------------------------------------------
    void WARN(const char* log_format, ...) __attribute__((format(printf,2,3)))
    {
        // filter overloaded log items
        if((time(0) - m_lastFlushTime == 0) && (m_warnItemNum > 10))
            return;
        ++ m_warnItemNum;

        va_list vargs;
        va_start(vargs, log_format);
        _log(LOG_PRI_WARN, LOG_HL_NONE, log_format, &vargs);
        va_end(vargs);
    }

    // ------------------------------------------------
    // Log a new INFO item
    // ------------------------------------------------
    void INFO(const char* log_format, ...) __attribute__((format(printf,2,3)))
    {
        if(LOG_PRI_INFO > m_ctx.log_level) {
            return;
        }
        va_list vargs;
        va_start(vargs, log_format);
        _log(LOG_PRI_INFO, LOG_HL_NONE, log_format, &vargs);
        va_end(vargs);
    }

    // ------------------------------------------------
    // Log a new DEBUG item
    // ------------------------------------------------
    void DEBUG(const char* log_format, ...) __attribute__((format(printf,2,3)))
    {
        if(LOG_PRI_DEBUG > m_ctx.log_level) {
            return;
        }
        va_list vargs;
        va_start(vargs, log_format);
        _log(LOG_PRI_DEBUG, LOG_HL_NONE, log_format, &vargs);
        va_end(vargs);
    }

    void Flush(bool force)
    {
        _flushLogQueue(force);
    }


    bool LogScreen()
    {
        return m_ctx.b_log_screen;
    }

    //mutex state can be cloned to child process
    //thread can not be cloned to child process
    //so if it has been logged, after fork, in child process need call this function
    void ReinitInForkChildProcess();

private:
    // filter log items
    std::atomic<time_t> m_lastFlushTime;
    std::atomic<int> m_warnItemNum;
    std::atomic<int> m_critItemNum;
    bool m_thread_is_create;
    bool m_exit;
    bool m_fork_process;
    int  m_log_fd;
    iovec m_iovs[MAX_LOG_QUEUE_LENGTH];
    list<string> m_work_cache;
    list<string> m_flush_cache;

    // Thread mutex lock for m_log_queue
    pthread_mutex_t m_mutex;
    pthread_mutex_t m_flush_mutex; // mutex for flush thread
    pthread_t m_thread_id;

    // Current log configuration context
    SLogContext m_ctx;
    FILE*  m_screen_stream;

    // ------------------------------------------------
    // Constructor
    // ------------------------------------------------
    CLog();

    // ------------------------------------------------
    // Copy Constructor
    // NOTE: DON'T implement it.
    // ------------------------------------------------
    CLog(const CLog&);

    // ------------------------------------------------
    // Operator Overloading
    // NOTE: DON'T implement it.
    // ------------------------------------------------
    CLog& operator=(const CLog&);

    // ------------------------------------------------
    // Destructor
    // ------------------------------------------------
    ~CLog();

    // ------------------------------------------------
    // Get thread ID
    // ------------------------------------------------
    static pid_t _gettid()
    {
        return syscall(SYS_gettid);
    }

    // ------------------------------------------------
    // Get current timestamp in sec or usec
    // ------------------------------------------------
    static long long _getTimestamp(bool b_usec = false)
    {
        timeval tv;
        gettimeofday(&tv, NULL);
        if(b_usec)
            return tv.tv_sec * 1000000 + tv.tv_usec;
        else
            return tv.tv_sec;
    }

    // ------------------------------------------------
    // Get parsed log color option
    // ------------------------------------------------
    int _getLogColor()
    {
        static const int log_colors[10] = {
            LOG_CLR_RED,     LOG_CLR_BLUE,
            LOG_CLR_GREEN,   LOG_CLR_LGREEN,
            LOG_CLR_LPURPLE, LOG_CLR_ORANGE,
            LOG_CLR_YELLOW,  LOG_CLR_CYAN,
            LOG_CLR_PURPLE,  LOG_CLR_LCYAN
        };
        if(m_ctx.log_color != LOG_CLR_PID && m_ctx.log_color != LOG_CLR_TID)
            return m_ctx.log_color;
        else {
            pid_t pid = m_ctx.log_color == LOG_CLR_TID ? _gettid() : getpid();
            return log_colors[pid % 10];
        }
    }

    // ------------------------------------------------
    // Configure logrotate
    // NOTE: Call it ONLY inside an external mutex lock
    // ------------------------------------------------
    void _configLogrotate(int fd);

    // ------------------------------------------------
    // Format a log item
    // ------------------------------------------------
    void _formatLogItem(char* buf, const int log_priority, const char* log_format, va_list* vargs, bool b_log_screen);

    // ------------------------------------------------
    // Print a log item to screen
    // ------------------------------------------------
    void _printLogItem(const char* buf, const int log_highlight);

    // ------------------------------------------------
    // Flush log queue to log files
    // NOTE: Call it ONLY inside an external mutex lock
    // ------------------------------------------------
    void _flushLogQueue(bool force = false);

    // ----------------------------------------------------------------
    // Log a new item internally
    // ----------------------------------------------------------------
    void _log(const int log_priority, const int log_highlight, const char* log_format, va_list* vargs);

    void _flushLogImp(int fd);
    void _flushLogInNewThreadWrapper();
    static void* _flushLogInNewThread(void* arg);
};


#define LOG_DEBUG_P(s,...)    CLog::getInstance().DEBUG(s, ##__VA_ARGS__)//LOG_DEBUG macro has been usesd
#define LOG_INFO_P(s, ...)    CLog::getInstance().INFO(s, ##__VA_ARGS__)//LOG_INFO macro has been used
#define LOG_WARN(s,...)     CLog::getInstance().WARN(s, ##__VA_ARGS__)
#define LOG_CRITICAL(s,...) CLog::getInstance().CRITICAL(s, ##__VA_ARGS__)

#define LOG_DEBUG_LIMIT(s,...)  \
    do {\
        static time_t _lastTime = 0; \
        time_t _now = time(NULL); \
        if (_now - _lastTime > 60) { \
            _lastTime = _now; \
            CLog::getInstance().DEBUG(s, ##__VA_ARGS__); \
        } \
    }while(0)

#define LOG_INFO_LIMIT(s,...)  \
    do { \
        static time_t _lastTime = 0; \
        time_t _now = time(NULL); \
        if (_now - _lastTime > 60) { \
            _lastTime = _now; \
            CLog::getInstance().INFO(s, ##__VA_ARGS__); \
        } \
    }while(0)

#define LOG_WARN_LIMIT(s,...)  \
    do { \
        static time_t _lastTime = 0; \
        time_t _now = time(NULL); \
        if (_now - _lastTime > 60) { \
            _lastTime = _now; \
            CLog::getInstance().WARN(s, ##__VA_ARGS__); \
        } \
    }while(0)

#define LOG_CRITICAL_LIMIT(s,...)  \
    do {\
        static time_t _lastTime = 0; \
        time_t _now = time(NULL); \
        if (_now - _lastTime > 60) { \
            _lastTime = _now; \
            CLog::getInstance().CRITICAL(s, ##__VA_ARGS__); \
        } \
    }while(0)

#endif
