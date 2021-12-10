#include "kv_store.h"
int check_dev(kv_dev_t *dev) {
	if (!dev)
		return -1;
	return 0;
}
int check_dev_write(kv_dev_t *dev) {
	struct mem *memtable = dev->memtable;
	if (!memtable) {
		printf("memtable is null\n");
		return -1;
	}
	DataBlock *blk = dev->w_block;
	if (!blk) {
		printf("block is null\n");
		return -1;
	}
	if((blk->block.type != STAR_BLOCK_TYPE_DATA)\
        || (blk->block.size != DATA_BLOCK_SIZE) || (blk->block.start_address != (blk->block.id * DATA_BLOCK_SIZE))\
        || (blk->block.end_address != ((blk->block.id + 1) * DATA_BLOCK_SIZE))){
		printf("block is wrong:%d \n", blk->block.id);
		return	-1;
    }
	return 0;
}
void printf_dev_header(dev_header_t *devheader) {
	printf("ssd size:%ld--total_blocks:%ld\n", devheader->ssd_size, devheader->total_blocks);
	printf("dbid:%d--sector_size:%d\n", devheader->db_id, devheader->sector_size);
	printf("bitblock:start:%d--end:%d\n", devheader->bit_blockid_start, devheader->bit_blockid_end);
	printf("memblock:start:%d--end:%d\n", devheader->mem_blockid_start, devheader->mem_blockid_end);
	printf("datablock:start:%d--end:%d\n", devheader->data_blockid_start, devheader->data_blockid_end);
	printf("devname:%s--dbname:%s\n", devheader->devname, devheader->dbname);
}
kv_dev_t *kv_create_dev(char *dev, char *dbname, int dbindex)
{
	int err, fd;
	int64_t ssdSizeByte = 0;
	int sector_size;
	struct stat nvme_stat;
	if ((fd = open(dev, O_RDWR | __O_DIRECT)) < 0) {
		printf("io open :%s err\n", dev);
		goto open_err;
	}
	if((err = fstat(fd, &nvme_stat)) < 0)
		goto stat_err;
	if (!S_ISCHR(nvme_stat.st_mode) && !S_ISBLK(nvme_stat.st_mode))
		goto dev_err;
	if (ioctl(fd, BLKPBSZGET, &sector_size) < 0) {
		printf("io sector_size err\n");
		goto ioctl_err;
	};
	if (ioctl(fd, BLKGETSIZE64, &ssdSizeByte) < 0) {
		printf("io ssdSizeByte err\n");
		goto ioctl_err;
	}
	kv_dev_t *sdev = (kv_dev_t *)malloc(sizeof(kv_dev_t));
	if (!sdev) {
		goto malloc_err;
	}
	sdev->header = malloc(sizeof(dev_header_t));
	if (!sdev->header) {
		goto malloc_err;
	}
	memset(sdev->header, 0 , sizeof(dev_header_t));
	sdev->fd = fd;
	sdev->header->db_id = dbindex;
	sdev->header->sector_size = sector_size;
	sdev->header->ssd_size = ssdSizeByte;
	sdev->header->total_blocks = ssdSizeByte / DATA_BLOCK_SIZE;
	strcpy(sdev->header->devname, dev);
	strcpy(sdev->header->dbname, dbname);
	return sdev;
ioctl_err:
dev_err:
stat_err:
	close(fd);
malloc_err:
open_err:
	return NULL;
}
int kv_init_dev(kv_dev_t *dev, int radio){
	int ret = KV_STORE_SUCCESS;
	if (check_dev(dev) < 0) {
		ret = KV_STORE_INVALID_PARAMETER;
		goto err;
	}
	dev->bits = bit_create(dev->fd);
	if (!dev->bits) {
		ret = KV_STORE_ERROR_BITMAP;
		goto err;
	}
	bit_set_block_status(dev->bits, STAR_DBINFO_BLOCKID, true);

	dev->header->bit_blockid_start = BLOCK_START;
	int bit_block_nums = dev->bits->bit_of_blocks;
	dev->header->bit_blockid_end = dev->header->bit_blockid_start + bit_block_nums - 1;

	dev->header->mem_blockid_start =dev->header->bit_blockid_end + 1;
	int mem_block_nums = (dev->header->total_blocks - dev->bits->bit_of_blocks) / radio;
	dev->header->mem_blockid_end = dev->header->mem_blockid_start + mem_block_nums - 1;

	dev->header->data_blockid_start = dev->header->mem_blockid_end + 1;
	int data_block_nums = (dev->header->total_blocks - dev->bits->bit_of_blocks - mem_block_nums);
	dev->header->data_blockid_end = dev->header->data_blockid_start + data_block_nums - 1;
	dev->memtable = mem_restore(dev->fd, dev->header->mem_blockid_start, dev->header->mem_blockid_end);
	if (!dev->memtable) {
		ret = KV_STORE_ERROR_SKIPLIST;
		goto err;
	}
	int block_id = get_available_block_id(dev->bits, STAR_BLOCK_TYPE_DATA, dev->header->data_blockid_start, dev->header->data_blockid_end);
	if ((block_id < dev->header->data_blockid_start) || (block_id > dev->header->data_blockid_end)) {
			ret = KV_STORE_INVALID_PARAMETER;
			goto err;	
	}
	dev->w_block = stardb_create_data_block(block_id);
	if(!dev->w_block){
		ret = KV_STORE_INVALID_PARAMETER;
		goto err;	
	}
err:
	return ret;
}
int kv_store_devheader(kv_dev_t *dev) {
	int ret = KV_STORE_SUCCESS;
	DataBlock *info_block = stardb_create_data_block(STAR_DBINFO_BLOCKID);
	if(!info_block){
		ret = KV_STORE_INVALID_PARAMETER;
		return ret;
	}
	memcpy(info_block->data, dev->header, sizeof(dev_header_t));
	ret = stardb_store_data_block(dev->fd, info_block);
	if(ret < 0){
		ret = KV_STORE_INVALID_BLOCK;
		return ret;
	}
	free(info_block);
	return ret;
}
dev_header_t *kv_restore_devheader(int fd) {
	DataBlock *info_block = stardb_restore_data_block(fd, STAR_DBINFO_BLOCKID);
	if(!info_block){
		goto err;
	}
	dev_header_t *header = malloc (sizeof(dev_header_t));
	if(!header) {
		goto err;
	}
	memset(header, 0, sizeof(dev_header_t));
	memcpy(header, info_block->data, sizeof(dev_header_t));
	free(info_block);
	return header;
err:
	if (info_block) free(info_block);
	return NULL;
}
kv_dev_t *kv_restore_dev(char *dev, int nsid) {
	int fd;
	if ((fd = open(dev, O_RDWR | __O_DIRECT)) < 0) {
		goto err;
	}
	kv_dev_t *sdev = (kv_dev_t *)malloc(sizeof(kv_dev_t));
	if (!sdev) {
		goto err;
	}
	sdev->fd = fd;
	sdev->header = kv_restore_devheader(sdev->fd);
	if (!sdev->header) {
		goto err;
	}
	if ((sdev->header->db_id != nsid) || (strcmp(sdev->header->devname, dev) != 0)) {
		printf("db id is wrong restore: %d\n", sdev->header->db_id);
		goto err;
	}
	sdev->bits = bit_read(sdev->fd, sdev->header->bit_blockid_start, sdev->header->bit_blockid_end);
	if (!sdev->bits) {
		goto err;
	}
	sdev->memtable = mem_restore(sdev->fd, sdev->header->mem_blockid_start, sdev->header->mem_blockid_end);
	if (!sdev->memtable) {
		goto err;
	}
	int block_id = get_available_block_id(sdev->bits, STAR_BLOCK_TYPE_DATA, sdev->header->data_blockid_start, sdev->header->data_blockid_end);
	if ((block_id < sdev->header->data_blockid_start) || (block_id > sdev->header->data_blockid_end)) {
		goto err;	
	}
	sdev->w_block = stardb_get_data_block(sdev->fd, block_id);
	if(!sdev->w_block){
		goto err;	
	}
	return sdev;
err:
	if(fd) close(fd);
	if(sdev->header) free(sdev->header);
	if(sdev->memtable) mem_destroy(sdev->memtable);
	if(sdev->w_block) free(sdev->w_block);
	if(sdev) free(sdev);
	return NULL;
}
int kv_close_dev(kv_dev_t * dev){
	int ret = KV_STORE_SUCCESS;
	ret = kv_store_devheader(dev);
	if(ret < 0){
		ret = KV_STORE_ERROR_WRITE;
		return ret;
	}
	ret = bit_save(dev->fd, dev->bits, dev->header->bit_blockid_start, dev->header->bit_blockid_end);
	if(ret < 0){
		ret = KV_STORE_ERROR_BITMAP;
		return ret;
	}
	ret = mem_flush(dev->fd, dev->memtable, dev->header->mem_blockid_start, dev->header->mem_blockid_end);
	if (ret != KV_STORE_SUCCESS) {
		return ret;
	}
	stardb_store_data_block(dev->fd, dev->w_block);
	if (ret != KV_STORE_SUCCESS) {
		return ret;
	}
	return close(dev->fd);
}
void kv_destroy_dev(kv_dev_t * dev){
	if (dev->bits)
		free(dev->bits);
	if (dev->header)
		free(dev->header);
	if(dev->memtable)
		mem_destroy(dev->memtable);
	if (dev)
		free(dev);
	return;
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
		block_id = get_available_block_id(dev->bits, STAR_BLOCK_TYPE_DATA, dev->header->data_blockid_start, dev->header->data_blockid_end);
		if (block_id == STAR_DATA_BLOCKID_END) {
			ret = KV_STORE_ERROR_WRITE;
			goto err;		
		}
		stardb_clear_data_block(dev->fd, block_id, block);
	}
	size_t offset = data_block_add_rowdata(block, rowdata);
	free(rowdata);
	ret = stardb_store_data_block(dev->fd, block);
	if (ret < 0 ) {
		ret = KV_STORE_ERROR_WRITE;
		goto err;	
	}
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