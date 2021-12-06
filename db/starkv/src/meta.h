#ifndef _META_H
#define _META_H

#include <stdint.h>
#include <pthread.h>
#include "os.h"
#define FILE_NAME_SIZE (256) /* file name size */
#define META_NODE_SIZE sizeof(struct meta_node)
#define NESSDB_MAX_KEY_SIZE (24) /* max key size */
#define META_MAX_COUNT (1000) /* max meta count */
struct meta_node{
	char end[MAX_ROW_SIZE];
	uint32_t count;
	size_t end_size;
	uint32_t block_id;
	uint32_t lsn;
};

struct meta {
	uint32_t sn;
	uint32_t size;
	struct meta_node nodes[META_MAX_COUNT];
};
struct meta *meta_new();
void meta_set(struct meta *meta, struct meta_node *node);
void meta_set_byname(struct meta *meta, struct meta_node *node);
void meta_free(struct meta *meta);
struct meta_node *meta_get(struct meta *meta, char *key);
void meta_print(struct meta *meta);
int meta_flush(int fd, struct meta *meta);
struct meta *meta_restore(int fd);
struct meta * metaInit(int fd);
void meta_set_node(struct meta *meta, char *max_key, size_t max_key_len, int count, int block_id);
void meta_modify_node(struct meta *meta, char *max_key, size_t max_key_len, int count, int block_id);
#endif
