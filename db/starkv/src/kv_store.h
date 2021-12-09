#ifndef _KV_STORE_H
#define _KV_STORE_H

#ifdef __cplusplus
extern "C" {
#endif
#include "skiplist.h"
#include "util.h"
#include "data_block.h"
#include "bitmap.h"
#include "mem.h"
#include "debug.h"
#define MAX_SHORT_VAL_LEN 16777215
#define ZS_KEY_BASE_REC_SIZE 24
#define ZS_VAL_BASE_REC_SIZE 16
#define MAX_KEY_LEN 512
#define BLOCK_END 0xFFFFFFFF
typedef struct dev_header {
    int64_t  ssd_size;
	int64_t  total_blocks;
	uint32_t sector_size;
    uint32_t block_size;
	uint32_t bit_blockid_start;
	uint32_t bit_blockid_end;
	uint32_t mem_blockid_start;
	uint32_t mem_blockid_end;
	uint32_t data_blockid_start;
	uint32_t data_blockid_end;
	uint32_t db_id;
	char dbname[32];
	char devname[32];
}dev_header_t;

typedef struct kv_dev{
	int fd;
	dev_header_t *header;
	DataBlock    *w_block;
    struct mem   *memtable;
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
enum record_type_t {
        REC_TYPE_KEY_DEL             =  0,
        REC_TYPE_KEY_ADD             =  1
};
typedef struct v_index{
    int value_type;
    int value_block_id;
    int value_len;
    int value_offset;
}v_index;
kv_dev_t *kv_store_open(char *dev);
kv_dev_t *kv_create_dev(char *dev, char *dbname, int dbindex);
kv_dev_t *kv_restore_dev(char *dev, int nsid);
int kv_init_dev(kv_dev_t *dev, int radio);
dev_header_t * kv_restore_devheader(int fd);
int kv_store_devheader(kv_dev_t *dev);
int kv_close_dev(kv_dev_t *dev);
void kv_destroy_dev(kv_dev_t * dev);
int kv_store_flush(kv_dev_t *dev);
int kv_store_restore(kv_dev_t *dev);
int kv_store_put(kv_dev_t *dev, unsigned char *key, size_t keylen,unsigned char *data, size_t datalen, char **errptr);
int kv_store_get(kv_dev_t *dev, unsigned char *key, size_t keylen, unsigned char **buf, size_t *buflen, char **errptr);
bool SaveError(char** errptr, uint32_t status);
SDataRow kv_prepare_key(unsigned char *key, size_t keylen, int block_id, size_t data_offset, size_t datalen);
SDataRow kv_prepare_data(unsigned char *data, size_t datalen);
typedef struct KVIterator {
	int fd;
	SSkipListIterator *siter;
}KVIterator;

void* kv_create_iter(kv_dev_t * dev);
void  kv_destroy_iter(void* iter);
bool kv_iter_next(void* siter);
char* kv_iter_key(void* iter, size_t* klen);
char* kv_iter_value(void* iter, size_t* klen);
#ifdef __cplusplus
}
#endif

#endif /* _LIB_NVMED_H */