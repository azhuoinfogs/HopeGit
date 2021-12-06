#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "meta.h"

struct meta *meta_new()
{
	struct meta *m = calloc(1, sizeof(struct meta));
	m->sn = 1;
	m->size = 0;
	return m;
}

int meta_flush(int fd, struct meta *meta) {
	size_t meta_size = sizeof(struct meta);
    uint32_t block_nums = (meta_size / SECTOR_SIZE) + (((meta_size % SECTOR_SIZE) == 0) ? 0 : 1);
    uint32_t meta_block_size = block_nums * SECTOR_SIZE;
    void *buf;
    if (!posix_memalign((void **)&buf, SECTOR_SIZE, meta_block_size)) {
		memset(buf, 0, meta_block_size);
	}
    else{
        printf("alloc posix_memalign feild\n");
        goto err;
    }
	memcpy(buf, meta, meta_size);
	if(pwrite(fd, buf, meta_block_size, STAR_WAL_BLOCKID_START * DATA_BLOCK_SIZE) <= 0){
        printf("Write block to ssd feild\n");
        goto err;
    }
	return 0;
err:
	return -1;
}

struct meta * meta_restore(int fd) {
	size_t meta_size = sizeof(struct meta);
    uint32_t block_nums = (meta_size / SECTOR_SIZE) + (((meta_size % SECTOR_SIZE) == 0) ? 0 : 1);
    uint32_t meta_block_size = block_nums * SECTOR_SIZE;
	void *buf;
    if (!posix_memalign((void **)&buf, SECTOR_SIZE, meta_block_size)) {
        memset(buf, 0, meta_block_size);
    } else{
        printf("alloc posix_memalign feild\n");
        goto err;
    }
    uint32_t read_bytes = pread(fd, buf, meta_block_size, STAR_WAL_BLOCKID_START * DATA_BLOCK_SIZE);
	if (read_bytes != meta_block_size) {
        printf("pread meta failed\n");
        goto err;
	}
	struct meta *m = (struct meta*)buf;
	if ((m->sn != 1) ) {
        printf("meta is invalid\n");
        goto err;
	}
	return m;
err:
	if (buf) free(buf);
	return NULL;
}

void meta_set(struct meta *meta, struct meta_node *node)
{
	size_t left = 0, right = meta->size;
	while (left < right) {
		size_t i = (right - left) / 2 + left;
		int cmp = strcmp(node->end, meta->nodes[i].end);
		if(cmp == 0) {
			memcpy(meta->nodes[i].end, node->end, NESSDB_MAX_KEY_SIZE);
			return;
		}
		if(cmp < 0)
			right = i;
		else 
			left = i + 1;
	}
	size_t i = left;
	meta->size++;
	node->lsn = meta->size;
	memmove(&meta->nodes[i + 1], &meta->nodes[i], (meta->size - i) * META_NODE_SIZE);
	memcpy(&meta->nodes[i], node, META_NODE_SIZE);
}
void meta_set_node(struct meta *meta, char *max_key, size_t max_key_len, int count, int block_id) {
		struct meta_node *node = calloc(1, sizeof(struct meta_node));
		memcpy(node->end, max_key, max_key_len);
		node->end_size = max_key_len;
		node->count = count;
		node->block_id = block_id;
		meta_set(meta, node);
}

void meta_free(struct meta *meta)
{
	if(meta)
		free(meta);
}

struct meta_node *meta_get(struct meta *meta, char *key)
{
	uint32_t left = 0, right = meta->size, i;
	while(left < right) {
		i = (right - left) / 2 + left;
		int cmp = strcmp(key, meta->nodes[i].end);
		if(cmp == 0)
			return &meta->nodes[i];
		if(cmp < 0)
			right = i;
		else 
			left = i + 1;
	}
	i = left;
	if(i == meta->size)
		return NULL;

	return &meta->nodes[i];
}
void meta_modify_node(struct meta *meta, char *max_key, size_t max_key_len, int count, int block_id)
{
	int index = meta->size -1;
	memcpy(meta->nodes[index].end, max_key, max_key_len);
	meta->nodes[index].count = count;
	meta->nodes[index].end_size = max_key_len;
	meta->nodes[index].block_id = block_id;
}

void meta_print(struct meta *meta) {
	printf("meta size:%d--meta lsn:%d\n",meta->size, meta->sn);
	for (int i =0; i < meta->size; i++) {
		struct meta_node node = (struct meta_node ) meta->nodes[i];
		printf("end:%s, id:%d count:%d lsn:%d\n", node.end, node.block_id, node.count, node.lsn);
	}
}
struct meta * metaInit(int fd)
{
    struct meta *meta= meta_restore(fd);
    if(!meta){
        fprintf(stdout, "Create new meta\n");
        meta = meta_new(fd);
        if(!meta){
            fprintf(stdout, "meta create failed\n");
            return NULL;
        }
        int res = meta_flush(fd, meta);
        if(res  < 0) {
            printf("meta save failed, res %d\n", res); 
            return NULL; 
        }        
    }
    return meta;
}
