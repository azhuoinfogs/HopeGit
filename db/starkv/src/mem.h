#ifndef _MEM_H
#define _MEM_H

#include <stdint.h>
#include <pthread.h>
#include "skiplist.h"
#include "kv_store.h"
#include "os.h"
#include "data_block.h"
#define FILE_NAME_SIZE (256) /* file name size */
#define META_NODE_SIZE sizeof(struct meta_node)
#define NESSDB_MAX_KEY_SIZE (24) /* max key size */
#define META_MAX_COUNT (1000) /* max meta count */
#define STAR_DATA_SKIPLIST_LEVEL 5
typedef int32_t VarDataOffsetT;

struct mem{
	SSkipList *skipList;
    int64_t mem_size;
};
struct mem *mem_create();
struct mem * mem_restore(int fd);
int mem_flush(int fd, struct mem *mem);
int mem_recreate(struct mem *mem);
int mem_put(struct mem *mem, SDataRow row);
SDataRow mem_get(struct mem *mem, unsigned char *key, size_t keylen);
int mem_full(struct mem *mem, SDataRow row);
void mem_print(struct mem *mem);
SDataRow mem_get_max(struct mem *mem);
SDataRow mem_put_and_pop_max(struct mem *mem, SDataRow row);
#endif
