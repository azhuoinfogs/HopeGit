#ifndef _LIB_NVMED_H
#define _LIB_NVMED_H

#ifdef __cplusplus
extern "C" {
#endif
#include<inttypes.h>
#include<assert.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/ioctl.h>
#include<linux/fs.h>
#include<errno.h>
#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "skiplist.h"
#include "util.h"
#include "meta.h"
#include "os.h"
#include "data_block.h"
#include "bitmap.h"
#include "mem.h"
#include "debug.h"
typedef struct kv_dev{
    int fd;
    int create_as_new;
    int64_t ssd_size;
    int sector_size;
	uint32_t cur_block_id;
    struct meta *meta;
    struct mem *memtable;
	BITS bits;
}kv_dev_t;

enum {
	KV_STORE_SUCCESS			= 0x0,
	KV_STORE_ERROR		        = 0x1,
	KV_STORE_INVALID_DEV		= 0x2,
	KV_STORE_INVALID_PARAMETER	= 0x3,
	KV_STORE_INVALID_MALLOC		= 0x4,
	KV_STORE_ERROR_SKIPLIST		= 0x5,
	KV_STORE_INVALID_DB		    = 0x6,
	KV_STORE_ERROR_WRITE	    = 0x7,
	KV_STORE_ERROR_READ	        = 0x8,
	KV_STORE_INVALID_KEY	    = 0x9,
	KV_STORE_INVALID_BLOCK		= 0x10,
	KV_STORE_BLOCK_FULL		    = 0x11,
	KV_STORE_ERROR_BITMAP		= 0x12,
};
kv_dev_t *kv_store_open(char *devname);
int kv_store_init(kv_dev_t *dev, int create_as_new);
int kv_store_close(kv_dev_t *dev);
void kv_store_free(kv_dev_t * dev);
int kv_store_flush(kv_dev_t *dev);
int kv_store_restore(kv_dev_t *dev);
int kv_store_put(kv_dev_t *dev, unsigned char *key, size_t keylen,unsigned char *data, size_t datalen, char **errptr);
int kv_store_get(kv_dev_t *dev, unsigned char *key, size_t keylen, unsigned char **buf, size_t *buflen, char **errptr);
bool SaveError(char** errptr, uint32_t status);
SDataRow kv_prepare_key(unsigned char *key, size_t keylen, int block_id, size_t data_offset, size_t datalen);
SDataRow kv_prepare_data(unsigned char *data, size_t datalen);
#ifdef __cplusplus
}
#endif

#endif /* _LIB_NVMED_H */