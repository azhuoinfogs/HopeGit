#include "kv_store.h"
static int sector_size;
kv_dev_t *kv_store_open(char *dev)
{
	int err, fd;
	int64_t _ssdSizeByte = 0;
	struct stat nvme_stat;
	if ((fd = open(dev, O_RDWR | __O_DIRECT)) < 0)
		goto open_err;
	if((err = fstat(fd, &nvme_stat)) < 0)
		goto stat_err;
	if (!S_ISCHR(nvme_stat.st_mode) && !S_ISBLK(nvme_stat.st_mode))
		goto dev_err;
	if (ioctl(fd, BLKPBSZGET, &sector_size) < 0) {
		goto ioctl_err;
	};
	if (ioctl(fd, BLKGETSIZE64, &_ssdSizeByte) < 0) {
		goto ioctl_err;
	}
	kv_dev_t *sdev = (kv_dev_t *)malloc(sizeof(kv_dev_t));
	if (!sdev) {
		goto malloc_err;
	}
	sdev->fd = fd;
	sdev->sector_size = sector_size;
	sdev->ssd_size = _ssdSizeByte;
	return sdev;
ioctl_err:
dev_err:
stat_err:
	close(fd);
malloc_err:
open_err:
	return NULL;
}
int check_dev(kv_dev_t *dev) {
	return 0;
}
int check_dev_write(kv_dev_t *dev) {
	DataBlock *blk = dev->w_block;
	if (!blk) {
		return -1;
	}
	struct mem *memtable = dev->memtable;
	if (!memtable) {
		return -1;
	}
	if((blk->block.type != STAR_BLOCK_TYPE_DATA)\
        || (blk->block.size != DATA_BLOCK_SIZE) || (blk->block.start_address != (blk->block.id * DATA_BLOCK_SIZE))\
        || (blk->block.end_address != ((blk->block.id + 1) * DATA_BLOCK_SIZE))){
		return	-1;
    }
	return 0;
}
void kv_store_free(kv_dev_t * dev){
	if (dev->bits)
		free(dev->bits);
	if (skipListGetSize(dev->memtable->skipList))
    	skipListDestroy(dev->memtable->skipList);
	if (dev->memtable)
		free(dev->memtable);
	if (dev)
		free(dev);
}
int kv_store_flush(kv_dev_t * fd) {
	return 0;
}
SDataRow kv_prepare_key(unsigned char *key, size_t keylen, int block_id, size_t data_offset, size_t datalen)
{
	int key_size = sizeof(VarDataLenT) + keylen;
    int row_size = SD_DATA_ROW_HEAD_SIZE + key_size + sizeof(struct v_index);
	
	tstr *keybuf =  calloc(1, key_size);
	keybuf->len = keylen;
	memcpy(keybuf->data, key, keylen);
	v_index v;
    v.value_block_id = block_id;
    v.value_offset = data_offset;
    v.value_len = datalen;
    v.value_type = REC_TYPE_KEY_ADD;
	SDataRow row = calloc(1, row_size);
	if(!row) {
		return NULL;
	}
	dataRowSetLen(row, row_size);
  	dataRowSetVersion(row, 1);
	void *data = dataRowTuple(row);
    memcpy(data, keybuf, varDataTLen(keybuf));
    memcpy(data + varDataTLen(keybuf), &v, sizeof(struct v_index));
	return row;
}
SDataRow kv_prepare_data(unsigned char *data, size_t datalen)
{	
	int row_size = SD_DATA_ROW_HEAD_SIZE + datalen;
	SDataRow row = malloc(row_size);
	if(!row) {
		return NULL;
	}
	memset(row, 0, row_size);
	dataRowSetLen(row, row_size);
  	dataRowSetVersion(row, 1);
	void *buf = dataRowTuple(row);
	memcpy(buf, data, datalen);
	return row;
}

int kv_store_put(kv_dev_t *dev, unsigned char *key, size_t keylen, unsigned char *data, size_t datalen, char **errptr) {
	int ret = KV_STORE_SUCCESS;
	if ((check_dev_write(dev) < 0) || !key || !data || (keylen == 0) || (datalen == 0) ){
		ret = KV_STORE_INVALID_PARAMETER;
		goto err;
	}
	DataBlock *block = dev->w_block;
	uint32_t block_id = block->block.id;
	struct mem *memtable = dev->memtable;
	SDataRow rowdata = kv_prepare_data(data, datalen);
	if (data_block_is_full(block, dataRowLen(rowdata))) {
		stardb_store_data_block(dev->fd, block);
    	bit_set_block_status(dev->bits, block_id, true);
		block_id = get_available_block_id(dev->bits, STAR_BLOCK_TYPE_DATA);
		if (block_id == STAR_DATA_BLOCKID_END) {
			ret = KV_STORE_ERROR_WRITE;
			goto err;		
		}
		stardb_clear_data_block(dev->fd, block_id, block);
	}
	size_t offset = data_block_add_rowdata(block, rowdata);
	free(rowdata);
	stardb_store_data_block(dev->fd, block);
	SDataRow rowkey = kv_prepare_key(key, keylen, block_id, offset, datalen);
	ret = mem_put(memtable, rowkey);
	free(rowkey);
err:
	SaveError(errptr, ret);
	return ret;
}
int kv_store_get(kv_dev_t *dev, unsigned char *key, size_t keylen, unsigned char **buf, size_t *buflen, char **errptr) {
	int ret = KV_STORE_SUCCESS;
	struct mem *memtable = dev->memtable;
	if ((check_dev(dev) < 0) || !key  || (keylen ==0)){
		ret = KV_STORE_INVALID_PARAMETER;
		goto err;
	}
	SDataRow sdata = mem_get(memtable, key, keylen);
	if (!sdata) {
		ret = KV_STORE_ERROR_SKIPLIST;
		__DEBUG(LEVEL_DEBUG, "mem_get %s not exist", key);
		goto err;
	}
	void *gdata = dataRowTuple(sdata);
    tstr *da = (tstr*) gdata;
    struct v_index * dv = (struct v_index *)(gdata + varDataTLen(gdata));
	DataBlock *get_block = stardb_get_data_block(dev->fd, dv->value_block_id);
	SDataRow gbuf =data_block_get_rowdata(get_block, dv->value_offset);
	int length = dataRowLen(gbuf) - SD_DATA_ROW_HEAD_SIZE;
	*buf = calloc(1, length);
	if (!*buf) {
		ret = KV_STORE_INVALID_MALLOC;
		goto err;
	}
	void *vbuf = dataRowTuple(gbuf);
	memcpy(*buf, vbuf, length);
	*buflen = length;
	free(get_block);
err:
	SaveError(errptr, ret);
	return ret;
}
int kv_store_init(kv_dev_t *dev, int create_as_new){
	int ret = KV_STORE_SUCCESS;
	if (check_dev(dev) < 0) {
		ret = KV_STORE_INVALID_PARAMETER;
		goto err;
	}
	dev->bits = bitInit(dev->fd);
	if (!dev->bits) {
		ret = KV_STORE_ERROR_BITMAP;
		goto err;
	}
	dev->memtable = mem_restore(dev->fd);
	if (!dev->memtable) {
		ret = KV_STORE_ERROR_SKIPLIST;
		goto err;
	}
	int block_id = get_available_block_id(dev->bits, STAR_BLOCK_TYPE_DATA);
	if ((block_id < STAR_DATA_BLOCKID_START) || (block_id > STAR_DATA_BLOCKID_END)) {
			ret = KV_STORE_INVALID_PARAMETER;
			goto err;	
	}
	dev->w_block = stardb_get_data_block(dev->fd, block_id);
	if(!dev->w_block){
		ret = KV_STORE_INVALID_PARAMETER;
		goto err;	
	}
err:
	return ret;
}

int kv_store_close(kv_dev_t * dev){
	bit_save(dev->fd, dev->bits);
	if (skipListGetSize(dev->memtable->skipList) > 0)
		mem_flush(dev->fd, dev->memtable);
	return close(dev->fd);
}

void *kv_create_iter(kv_dev_t * dev) {
	KVIterator *iter = malloc(sizeof(KVIterator));
	iter->fd = dev->fd;
	iter->siter = skipListCreateIter(dev->memtable->skipList);
	return (void *)iter;
}
void kv_destroy_iter(void* iter) {
    KVIterator* kIter = (KVIterator *)iter;
	skipListDestroyIter(kIter->siter);
	return;
}
bool kv_iter_next(void* iter) {
    KVIterator* kIter = (KVIterator *)iter;
    return skipListIterNext(kIter->siter);
}
char* kv_iter_key(void* iter, size_t* klen) {
    KVIterator* kIter = (KVIterator *)iter;
    SSkipListIterator* sIter = (SSkipListIterator *)kIter->siter;
    SSkipListNode *node = skipListIterGet(sIter);
	SDataRow sdata = SL_GET_NODE_DATA(node);
	void *gdata = dataRowTuple(sdata);
    tstr *da = (tstr*) gdata;
	char *key = calloc(1, da->len);
	memcpy(key, da->data, da->len);
	*klen = da->len;
    return key;
}
char* kv_iter_value(void* iter, size_t* klen) {
    KVIterator* kIter = (KVIterator *)iter;
    SSkipListIterator* sIter = (SSkipListIterator *)kIter->siter;
	int fd = kIter->fd;
    SSkipListNode *node = skipListIterGet(sIter);
	SDataRow sdata = SL_GET_NODE_DATA(node);
	void *gdata = dataRowTuple(sdata);
    struct v_index * dv = (struct v_index *)(gdata + varDataTLen(gdata));
	DataBlock *get_block = stardb_get_data_block(fd, dv->value_block_id);
	SDataRow gbuf = data_block_get_rowdata(get_block, dv->value_offset);
	int length = dataRowLen(gbuf) - SD_DATA_ROW_HEAD_SIZE;
	char *buf = calloc(1, length);
	void *vbuf = dataRowTuple(gbuf);
	memcpy(buf, vbuf, length);
	*klen = length;
	free(get_block);
	return buf;
}