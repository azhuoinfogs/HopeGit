#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <elog_file.h>
#include <elog_file_cfg.h>

/* initialize OK flag */
static bool init_ok = false;
static FILE *fp = NULL;
static int fd = -1;
static ElogFileCfg local_cfg;

ElogErrCode elog_file_init(void)
{
    ElogErrCode result = ELOG_NO_ERR;
    ElogFileCfg cfg;

    if (init_ok)
        goto __exit;

    elog_file_port_init();

    cfg.name = ELOG_FILE_NAME;
    cfg.max_size = ELOG_FILE_MAX_SIZE;
    cfg.max_rotate = ELOG_FILE_MAX_ROTATE;

    elog_file_config(&cfg);

    init_ok = true;
__exit:
    return result;
}

/*
 * Reopen file
 */
static bool elog_file_reopen(void)
{
    FILE *tmp_fp;

    tmp_fp = fopen(local_cfg.name, "a+");
    if (tmp_fp) {
        if (fp)
            fclose(fp);

        fp = tmp_fp;
        fd = fileno(fp);
        return true;
    }

    return false;
}

/*
 * rotate the log file xxx.log.n-1 => xxx.log.n, and xxx.log => xxx.log.0
 */
static void elog_file_rotate(void)
{
#define SUFFIX_LEN                     10
    /* mv xxx.log.n-1 => xxx.log.n, and xxx.log => xxx.log.0 */
    int n;
    char oldpath[256], newpath[256];
    size_t base = strlen(local_cfg.name);

    memcpy(oldpath, local_cfg.name, base);
    memcpy(newpath, local_cfg.name, base);

    for (n = local_cfg.max_rotate - 1; n >= 0; --n) {
        snprintf(oldpath + base, SUFFIX_LEN, n ? ".%d" : "", n - 1);
        snprintf(newpath + base, SUFFIX_LEN, ".%d", n);
        rename(oldpath, newpath);
    }
}

/*
 * Check if it needed retate
 */
static bool elog_file_retate_check(void)
{
    struct stat statbuf;
    statbuf.st_size = 0;
    if (stat(local_cfg.name, &statbuf) < 0)
        return false;

    if (statbuf.st_size > local_cfg.max_size)
        return true;

    return false;
}

void elog_file_write(const char *log, size_t size)
{
    ELOG_ASSERT(init_ok);
    ELOG_ASSERT(log);
    struct stat statbuf;

    statbuf.st_size = 0;

    elog_file_port_lock();

    fstat(fd, &statbuf);

    if (unlikely(statbuf.st_size > local_cfg.max_size)) {
#if ELOG_FILE_MAX_ROTATE > 0
	    if (elog_file_retate_check()) {
                /* rotate the log file */
                elog_file_rotate();
        }

        if (!elog_file_reopen()) {
            elog_file_port_unlock();
            return;
        }
#else
        return ;
#endif
    }

    fwrite(log, size, 1, fp);

#ifdef ELOG_FILE_FLUSH_CAHCE_ENABLE
    fflush(fp);
    fsync(fd);
#endif

    elog_file_port_unlock();
}

void elog_file_deinit(void)
{
    ELOG_ASSERT(init_ok);

    elog_file_port_deinit();
    fclose(fp);
}

void elog_file_config(ElogFileCfg *cfg)
{
    if (fp) {
        fclose(fp);
    }

    elog_file_port_lock();

    local_cfg.name = cfg->name;
    local_cfg.max_size = cfg->max_size;
    local_cfg.max_rotate = cfg->max_rotate;

    fp = fopen(local_cfg.name, "a+");
    if (fp)
        fd = fileno(fp);
    else
        fd = -1;

    elog_file_port_unlock();
}
