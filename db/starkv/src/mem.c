#include "mem.h"
static char *sdbGetTsTupleKey(const void *data) { return dataRowTuple(data); }
struct mem *mem_create()
{
	struct mem *m = calloc(1, sizeof(struct mem));
	if (!m) {
		goto err;
	}
	m->skipList = skipListCreate(STAR_DATA_SKIPLIST_LEVEL, STAR_DATA_TYPE_BINARY, sizeof(VarDataOffsetT), 0, 0, true, sdbGetTsTupleKey);
	if (!m->skipList) {
		goto err;
	}
	m->mem_size = 0;
	return m;
err:
	if (m) free(m);
	return NULL;
}
int mem_destroy(struct mem *mem){
	int ret = KV_STORE_SUCCESS;
	if (mem->skipList)
		skipListDestroy(mem->skipList);
	if (mem)
		free(mem);
	return ret;
}
int mem_flush(int fd, struct mem *mem, uint32_t blockid_start, uint32_t blockid_end)
{
	int ret = KV_STORE_SUCCESS;
  	int numOfRows = 0;
	DataBlock *block = stardb_create_data_block(blockid_start);
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
	int block_id = blockid_start;
    while (skipListIterNext(sIter))
    {
        SSkipListNode *node = skipListIterGet(sIter);
		SDataRow sdata = SL_GET_NODE_DATA(node);
		if (data_block_is_full(block, dataRowLen(sdata))) {
			stardb_store_data_block(fd, block);
			block_id++;
			if (block_id > blockid_end) {
				ret = KV_STORE_BLOCK_FULL;
				goto err;	
			}
			stardb_clear_data_block(fd, block_id, block);
		}
		void *gdata = dataRowTuple(sdata);
    	struct v_index * dv = (struct v_index *)(gdata + varDataTLen(gdata));
		if (dv->value_type == REC_TYPE_KEY_ADD) {
			data_block_add_rowkey(block, sdata);
			numOfRows++;
		}
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
struct mem * mem_restore(int fd, uint32_t mem_blockid_start, uint32_t mem_blockid_end){
	int ret = KV_STORE_SUCCESS;
	struct mem *m = mem_create(mem_blockid_start);
	if (!m) {
		ret = KV_STORE_INVALID_MALLOC;
		goto err;	
	}
	for (int block_id = mem_blockid_start; block_id < mem_blockid_end; block_id++) {
		DataBlock *block = stardb_restore_data_block(fd, block_id);
		if (!block) {
			//此block 以后的block 中无key.
			break;
		}
		void *buf = block->data;
		size_t offset = 0;
		size_t count = 0;
		while(true) {
			SDataRow s_record = (SDataRow )(block->data + offset);
			void *gdata = dataRowTuple(s_record);
        	tstr *da = (tstr*) gdata;
        	struct v_index * dv = (struct v_index *)(gdata + varDataTLen(gdata));
			if (dv->value_type != REC_TYPE_KEY_ADD) {
				break;
			}
			mem_put(m, s_record);
			count ++;
			offset += dataRowLen(s_record);
		}
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
int mem_full(struct mem *memtable, SDataRow row) {
	if ((memtable->mem_size + dataRowLen(row)) >= DATA_BLOCK_SIZE) {
		return 1;
	}
	return 0;
}
int mem_delete(struct mem *mem, unsigned char *key, size_t keylen) {
	tstr *keybuf = calloc(1, sizeof(tstr) + keylen);
	keybuf->len = keylen;
    memcpy(keybuf->data, key, keylen);
	int ret = skipListRemove(mem->skipList, (SSkipListKey)keybuf);
	free(keybuf);
	return ret;
}
SDataRow mem_get(struct mem *mem, unsigned char *key, size_t keylen) {
	tstr *keybuf = calloc(1, sizeof(tstr) + keylen);
	keybuf->len = keylen;
    memcpy(keybuf->data, key, keylen);
	SArray *nodes = skipListGet(mem->skipList, (SSkipListKey )keybuf);
    if ((nodes->size == 0) || (!nodes)) {
		return NULL;
	}
	SSkipListNode *pNode = (SSkipListNode *)starArrayGetP(nodes, 0);
	free(keybuf);
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
		void *gdata = dataRowTuple(sdata);
        tstr *da = (tstr*) gdata;
        struct v_index *dv = (struct v_index *)(gdata + varDataTLen(gdata));
		printf("%s-%d--%d\n", da->data, da->len, dv->value_block_id);
    }
}
