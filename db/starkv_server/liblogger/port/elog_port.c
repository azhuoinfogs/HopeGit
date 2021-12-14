#include <elog.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#ifdef ELOG_FILE_ENABLE
#include <elog_file.h>
#endif
static pthread_mutex_t output_lock;

/**
 * EasyLogger port initialize
 *
 * @return result
 */
ElogErrCode elog_port_init(void) {
    ElogErrCode result = ELOG_NO_ERR;

    pthread_mutex_init(&output_lock, NULL);

#ifdef ELOG_FILE_ENABLE
    elog_file_init();
#endif

    return result;
}

/**
 * output log port interface
 *
 * @param log output of log
 * @param size log size
 */
void elog_port_output(const char *log, size_t size) {
    /* output to terminal */
    printf("%.*s", (int)size, log);
#ifdef ELOG_FILE_ENABLE
    /* write the file */
    elog_file_write(log, size);
#endif 
}

/**
 * output lock
 */
void elog_port_output_lock(void) {
    pthread_mutex_lock(&output_lock);
}

/**
 * output unlock
 */
void elog_port_output_unlock(void) {
    pthread_mutex_unlock(&output_lock);
}


/**
 * get current time interface
 *
 * @return current time
 */
const char *elog_port_get_time(void) {
    static char cur_system_time[24] = { 0 };
    time_t timep;
    struct tm *p;

    time(&timep);
    p = localtime(&timep);
    if (p == NULL) {
        return "";
    }
    snprintf(cur_system_time, 18, "%02d-%02d %02d:%02d:%02d", p->tm_mon + 1, p->tm_mday,
            p->tm_hour, p->tm_min, p->tm_sec);

    return cur_system_time;
}

/**
 * get current process name interface
 *
 * @return current process name
 */
const char *elog_port_get_p_info(void) {
    static char cur_process_info[10] = { 0 };

    snprintf(cur_process_info, 10, "pid:%04d", getpid());

    return cur_process_info;
}

/**
 * get current thread name interface
 *
 * @return current thread name
 */
const char *elog_port_get_t_info(void) {
    static char cur_thread_info[10] = { 0 };

    snprintf(cur_thread_info, 10, "tid:%04ld", pthread_self());

    return cur_thread_info;
}
