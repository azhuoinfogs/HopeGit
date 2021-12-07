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
#define STAR_DATA_TYPE_NULL       0     // 1 bytes
#define STAR_DATA_TYPE_BOOL       1     // 1 bytes
#define STAR_DATA_TYPE_TINYINT    2     // 1 byte
#define STAR_DATA_TYPE_SMALLINT   3     // 2 bytes
#define STAR_DATA_TYPE_INT        4     // 4 bytes
#define STAR_DATA_TYPE_BIGINT     5     // 8 bytes
#define STAR_DATA_TYPE_FLOAT      6     // 4 bytes
#define STAR_DATA_TYPE_DOUBLE     7     // 8 bytes
#define STAR_DATA_TYPE_BINARY     8     // string
#define STAR_DATA_TYPE_TIMESTAMP  9     // 8 bytes
#define STAR_DATA_TYPE_NCHAR      10    // unicode string
typedef struct kv_dev{
    int fd;
    int create_as_new;
    int64_t ssd_size;
    int sector_size;
	DataBlock *w_block;
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