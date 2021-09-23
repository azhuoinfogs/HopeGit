#ifndef __ELOG_FILE__H__
#define __ELOG_FILE__H__

#include <stdio.h>
#include <elog.h>

#ifdef __cplusplus
extern "C" {
#endif

/* EasyLogger file log plugin's software version number */
#define ELOG_FILE_SW_VERSION                "V1.0.0"
#ifdef linux
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#else
#define likely(x)   (x)
#define unlikely(x) (x)
#endif

typedef struct {
    char *name;              /* file name */
    size_t max_size;         /* file max size */
    int max_rotate;          /* max rotate file count */
} ElogFileCfg;

/* elog_file.c */
ElogErrCode elog_file_init(void);
void elog_file_write(const char *log, size_t size);
void elog_file_config(ElogFileCfg *cfg);
void elog_file_deinit(void);

/* elog_file_port.c */
ElogErrCode elog_file_port_init(void);
void elog_file_port_lock(void);
void elog_file_port_unlock(void);
void elog_file_port_deinit(void);

#ifdef __cplusplus
}
#endif

#endif
