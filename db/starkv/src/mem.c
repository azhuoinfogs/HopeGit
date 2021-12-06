#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mem.h"
char *sdbGetTsTupleKey(const void *data) {
    record_t *sdata = (record_t *)dataRowTuple(data);
	return (char *)sdata->key;
}
static void mem_destroy(struct mem *pTableData) {
  if (pTableData) {
    skipListDestroy(pTableData->skipList);
    free(pTableData);
  }
}

static char *sdbGetTsTupleKey(const void *data) { return dataRowTuple(data); }
struct mem *mem_create()
{
	struct mem *m = calloc(1, sizeof(struct mem));
	if (!m) {
		goto err;
	}
	m->skipList = skipListCreate(100, STAR_DATA_TYPE_BINARY, sizeof(int32_t), 0, false, true, sdbGetTsTupleKey);
	if (!m->skipList) {
		goto err;
	}
	m->mem_size = 0;
	return m;
err:
	if (m) free(m);
	return NULL;
}
// int mem_recreate(struct mem *mem){
// 	int ret = KV_STORE_SUCCESS;
// 	skipListDestroy(mem->skipList);
// 	mem->skipList = NULL;
// 	mem->skipList = skipListCreate(100, STAR_DATA_TYPE_BINARY, sizeof(int32_t), 0, false, true, sdbGetTsTupleKey);
// 	if (!mem->skipList) {
// 		ret = KV_STORE_ERROR_SKIPLIST;
// 		goto err;
// 	}
// 	mem->mem_size = 0;
// err:
// 	return ret;
// }

// int mem_flushs(int fd, struct mem *mem, uint32_t block_id, unsigned char **buf, size_t *buflen, size_t *count)
// {
// 	int ret = KV_STORE_SUCCESS;
//   	int numOfRows = 0;
// 	DataBlock *block = stardb_create_data_block(block_id);
// 	if (!block) {
// 		ret = KV_STORE_INVALID_BLOCK;
// 		goto err;	
// 	}
// 	SSkipList *skipList = mem->skipList;
// 	SSkipListIterator *sIter = skipListCreateIter(skipList);
// 	if (!sIter) {
// 		ret = KV_STORE_ERROR_SKIPLIST;
// 		goto err;	
// 	}
//     while (skipListIterNext(sIter))
//     {
//         SSkipListNode *node = skipListIterGet(sIter);
// 		SDataRow sdata = SL_GET_NODE_DATA(node);
// 		data_block_add_rowdata(block, sdata);
// 		numOfRows++;
//     }
//   	ret = stardb_store_data_block(fd, block);
//   	if (ret < 0) {
// 		ret = KV_STORE_INVALID_BLOCK;
// 		goto err;
// 	}
// 	char *maxbuf = calloc(1, strlen( block->max_key));
// 	memcpy(maxbuf, block->max_key, strlen(block->max_key));
// 	*buflen = strlen(block->max_key);
// 	*count = block->row_count;
// 	*buf = maxbuf;
// err:
// 	if (sIter)
// 		skipListDestroyIter(sIter);
// 	return ret;
// }

int mem_flush(int fd, struct mem *mem)
{
	int ret = KV_STORE_SUCCESS;
  	int numOfRows = 0;
	DataBlock *block = stardb_create_data_block(STAR_MEM_START);
	if (!block) {
		ret = KV_STORE_INVALID_BLOCK;
		goto err;	
	}
	SSkipList *skipList = mem->skipList;
	SSkipListIterator *sIter = skipListCreateIter(skipList);
	if (!sIter) {
		ret = KV_STORE_ERROR_SKIPLIST;
		goto err;	
	}
	int block_id = STAR_MEM_START;

    while (skipListIterNext(sIter))
    {
        SSkipListNode *node = skipListIterGet(sIter);
		SDataRow sdata = SL_GET_NODE_DATA(node);
		if (data_block_is_full(block, dataRowLen(sdata))) {
			stardb_store_data_block(fd, block);
			block_id++;
			stardb_clear_data_block(fd, block_id, block);
		}
		data_block_add_rowkey(block, sdata);
		numOfRows++;
    }
  	ret = stardb_store_data_block(fd, block);
  	if (ret < 0) {
		ret = KV_STORE_INVALID_BLOCK;
		goto err;
	}
err:
	if (sIter)
		skipListDestroyIter(sIter);
	return ret;
}


// struct mem *mem_restore(int fd,  uint32_t block_id) {
// 	int ret = KV_STORE_SUCCESS;
// 	DataBlock *block = stardb_get_data_block(fd, block_id);
// 	if (!block) {
// 		ret = KV_STORE_INVALID_BLOCK;
// 		goto err;
// 	}
// 	struct mem *m = mem_create(block_id);
// 	if (!m) {
// 		ret = KV_STORE_INVALID_MALLOC;
// 		goto err;	
// 	}
// 	void *buf = block->data;
// 	int64_t total_mem_size = 0;
// 	size_t offset = 0;
// 	while(true) {
// 		record_t *record = (record_t *)(buf + offset);
// 		if (record->watermark != START_MARK) {
// 			break;
// 		}
// 		SDataRow row = kv_prepare(record);
// 		if (!row) {
// 			break;
// 		}
// 		int32_t level = 0;
//     	int32_t headSize = 0;
//     	skipListNewNodeInfo(m->skipList, &level, &headSize);
// 		SSkipListNode *pNode = calloc(1, headSize + dataRowLen(row));
// 		if (pNode == NULL) {
// 			break;
//     	}
//     	pNode->level = level;
// 		dataRowCpy(SL_GET_NODE_DATA(pNode), row);
//     	skipListPut(m->skipList, pNode);
// 		size_t mem_size = headSize + dataRowLen(row);
// 		m->mem_size += mem_size;
// 		offset += (sizeof(record_t)+ record->data_len);
// 	}
// 	if (block){
// 		stardb_free_data_block(block);
// 	}
// 	return m;
// err:
// 	if (block){
// 		stardb_free_data_block(block);
// 	}
// 	return NULL;
// }
struct mem *mem_restore(int fd) {
	int ret = KV_STORE_SUCCESS;
	struct mem *m = mem_create(STAR_MEM_START);
	if (!m) {
		ret = KV_STORE_INVALID_MALLOC;
		goto err;	
	}
	for (int block_id = STAR_MEM_START; block_id < STAR_MEM_END; block_id++) {
		DataBlock *block = stardb_restore_data_block(fd, block_id);
		if (!block) {
			//此block 以后的block 中无key.
			break;
		}
		void *buf = block->data;
		size_t offset = 0;
		while(true) {
			record_t *record = (record_t *)(buf + offset);
			if (record->watermark != START_MARK) {
				break;
			}
			SDataRow row = kv_prepare_data((void *)record, record->record_len);
			if (!row) {
				break;
			}
			mem_put(m, row);
			offset += record->record_len;
			free(row);
		}
		stardb_erase_data_block(fd, block_id, block);
		if (block){
			stardb_free_data_block(block);
		}
	}
	return m;
err:
	return NULL;
}
int mem_put(struct mem *mem, SDataRow row) {
	int ret = KV_STORE_SUCCESS;
	int32_t level = 0;
    int32_t headSize = 0;
	skipListNewNodeInfo(mem->skipList, &level, &headSize);
	size_t mem_size = headSize + dataRowLen(row);
	SSkipListNode *pNode = calloc(1, mem_size);
	if (pNode == NULL) {
		ret = KV_STORE_INVALID_MALLOC;
		goto err;
    }
    pNode->level = level;
	dataRowCpy(SL_GET_NODE_DATA(pNode), row);
    skipListPut(mem->skipList, pNode);
	mem->mem_size += mem_size;
err:
	return ret;
}
SDataRow mem_put_and_pop_max(struct mem *mem, SDataRow row) {
	int ret = KV_STORE_SUCCESS;
	int32_t level = 0;
    int32_t headSize = 0;
	size_t datalen = 0;
	skipListNewNodeInfo(mem->skipList, &level, &headSize);
	size_t mem_size = headSize + dataRowLen(row);
	SSkipListNode *pNode = calloc(1, mem_size);
	if (pNode == NULL) {
		ret = KV_STORE_INVALID_MALLOC;
		goto err;
    }
    pNode->level = level;
	dataRowCpy(SL_GET_NODE_DATA(pNode), row);
    skipListPut(mem->skipList, pNode);
	mem->mem_size += mem_size;

	SSkipListNode *pmaxNode = SL_GET_BACKWARD_POINTER((mem->skipList)->pTail, 0);
	SDataRow sdata = SL_GET_NODE_DATA(pmaxNode);
	datalen = dataRowLen(sdata);

	SDataRow pdata = calloc (1, datalen);
	memcpy(pdata, sdata, datalen);
	skipListRemoveNode(mem->skipList, pmaxNode);
	mem->mem_size -= datalen;
	return pdata;
err:
	return  NULL;
}
int mem_full(struct mem *memtable, SDataRow row) {
	if ((memtable->mem_size + dataRowLen(row)) >= DATA_BLOCK_SIZE) {
		return 1;
	}
	return 0;
}

SDataRow mem_get(struct mem *mem, unsigned char *key) {
	SArray *nodes = skipListGet(mem->skipList, key);
    if ((nodes->size == 0) || (!nodes)) {
		return NULL;
	}
	SSkipListNode *pNode = (SSkipListNode *)starArrayGetP(nodes, 0);
	return SL_GET_NODE_DATA(pNode);
}
void mem_print(struct mem *mem) {
    SSkipListIterator *sIter = skipListCreateIter(mem->skipList);
	printf("mem_size:%ld\n", mem->mem_size);
	printf("count:%ld\n", skipListGetSize(mem->skipList));
	// skipListPrint(mem->skipList,1);
    while (skipListIterNext(sIter))
    {
        SSkipListNode *node = skipListIterGet(sIter);
		SDataRow sdata = SL_GET_NODE_DATA(node);
		record_t *s_record = (record_t *)dataRowTuple(sdata);
        printf("mem:key:%s-keylen:%ld--value_len:%ld\n",  s_record->key,s_record->key_len, s_record->data_len);
        /* code */
    }
}
