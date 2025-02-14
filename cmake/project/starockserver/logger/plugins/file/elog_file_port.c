#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include <unistd.h>

#include <elog_file.h>
#include <elog_file_cfg.h>

#define ELOG_FILE_SEM_KEY   ((key_t)0x19910612)
#ifdef _SEM_SEMUN_UNDEFINED
union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                (Linux-specific) */
};
#endif

static int semid = -1;
static struct sembuf const up = {0, 1, SEM_UNDO};
static struct sembuf const down = {0, -1, SEM_UNDO};

static void lock_init(void);
static int lock_open(void);
/**
 * EasyLogger flile log pulgin port initialize
 *
 * @return result
 */
ElogErrCode elog_file_port_init(void) {
    ElogErrCode result = ELOG_NO_ERR;

    lock_init();

    return result;
}

/**
 * file log lock
 */
void inline elog_file_port_lock(void)
{
    semid == -1 ? -1 : semop(semid, (struct sembuf *)&down, 1);
}

/**
 * file log unlock
 */
void inline elog_file_port_unlock(void)
{
    semid == -1 ? -1 : semop(semid, (struct sembuf *)&up, 1);
}
/**
 * file log deinit
 */
void elog_file_port_deinit(void)
{

}

/**
 * initialize the lock 
 */
static void lock_init(void)
{
    int id, rc;
    union semun arg;
    struct sembuf sembuf;

    id = semget(ELOG_FILE_SEM_KEY, 1, IPC_CREAT | IPC_EXCL | 0666);
    if(likely(id == -1)) {
        id = lock_open();
        if (id == -1)
            goto __exit;
    } else {
        arg.val = 0;
        rc = semctl(id, 0, SETVAL, arg);
        if (rc == -1)
            goto __exit;

        sembuf.sem_num = 0;
        sembuf.sem_op = 1;
        sembuf.sem_flg = 0;

        rc = semop(id, &sembuf, 1);
        if (rc == -1)
            goto __exit;
    }

    semid = id;
__exit:
    return ;
}

/**
 * gets the lock 
 */
static int lock_open(void)
{
    int id, rc, i;
    union semun arg;
    struct semid_ds ds;

    id = semget(ELOG_FILE_SEM_KEY, 1, 0666);
    if(unlikely(id == -1))
        goto err;

    arg.buf = &ds;

    for (i = 0; i < 10; i++) {
        rc = semctl(id, 0, IPC_STAT, arg);
        if (unlikely(rc == -1))
            goto err;

        if(ds.sem_otime != 0)
            break;

        usleep(10 * 1000);
    }

    if (unlikely(ds.sem_otime == 0))
        goto err;

    return id;
err:
    return -1;
}
