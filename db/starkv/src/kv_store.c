#include "kv_store.h"
static int sector_size;
static int kv_store_write(kv_dev_t * dev, void *data, size_t data_size, size_t phys_sector_size, size_t offset)
{
	bool huge;
	int err = 0;
	__u32 block_count = data_size / phys_sector_size;
	__u32 block_offset = data_size % phys_sector_size;
	__u64 buffer_size = (block_count)*phys_sector_size;
	if (block_offset != 0) {
		buffer_size += phys_sector_size;
	}
	void *buffer = nvme_alloc(buffer_size, sector_size, &huge);
	if (!buffer) {
		fprintf(stderr, "can not allocate io payload\n");
		err = -ENOMEM;
		goto out;
	}
	memset(buffer, 0, buffer_size);
	memcpy(buffer, data, data_size);
	size_t res = pwrite(dev->fd, buffer, buffer_size, offset);
	if (res < 0) {
		fprintf(stderr, "can not pwrite buffer of device \n");
		err = -EFAULT;
		goto out;
	}
	if (buffer)
		nvme_free(buffer, huge);
	return res;
out:
	if (buffer)
		nvme_free(buffer, huge);
	return err;
}

static int kv_store_read(kv_dev_t *dev, void *buf, size_t data_size, size_t phys_sector_size, size_t offset)
{
	bool huge;
	int err = 0;

	__u32 block_count = data_size / phys_sector_size;
	__u32 block_offset = data_size % phys_sector_size;
	__u64 buffer_size = (block_count)*phys_sector_size;
	if (block_offset != 0) {
		buffer_size += phys_sector_size;
	}
	void *buffer = nvme_alloc(buffer_size,sector_size, &huge);
	if (!buffer) {
		fprintf(stderr, "can not allocate io payload\n");
		err = -ENOMEM;
		goto out;
	}
	memset(buffer, 0, buffer_size);
	size_t res = pread(dev->fd, buffer, buffer_size, offset);
	if (res < 0) {
		fprintf(stderr, "can not pwrite buffer of device \n");
		err = -EFAULT;
		goto out;
	}
	memcpy(buf, buffer, data_size);
	if (buffer)
		nvme_free(buffer, huge);
	return res;
out:
	if (buffer)
		nvme_free(buffer, huge);
	return err;
}

int kv_read_record_by_offset(kv_dev_t *dev, size_t offset, unsigned char **buf, size_t *buflen){
	int ret;
	bool huge;
	size_t keybuflen, valbuflen;
    size_t sector_size = dev->sector_size;

	size_t buffer_size = sector_size;
    void *rbuffer = nvme_alloc(buffer_size, sector_size, &huge);
	if (!rbuffer) {
		printf("can not allocate io payload\n");
        return -1;
	}
	ret = kv_store_read(dev, rbuffer, buffer_size, sector_size, offset);
	record_t *keybuf = (record_t *) rbuffer;
	if (keybuf->watermark != START_MARK) {
		printf("finished\n");
        return -1;
	}
	if (keybuf->record_len >= sector_size) {
		nvme_free(rbuffer, huge);
		rbuffer = NULL;
		keybuf = NULL;
		rbuffer = nvme_alloc(keybuf->record_len,sector_size, &huge);
		ret = kv_store_read(dev, rbuffer, buffer_size, sector_size, offset);
		keybuf = (record_t *) rbuffer;
	}
    unsigned char *kbuf;
	kbuf = calloc(1, keybuf->record_len);
	memcpy(kbuf, keybuf, keybuf->record_len);

	*buf = kbuf;
	*buflen = keybuf->record_len;
	nvme_free(rbuffer, huge);
	return ret;
}
// int kv_read_data_by_offset(kv_dev_t *dev, size_t offset, unsigned char **buf, size_t *buflen){
// 	int ret;
// 	bool huge;
// 	size_t keybuflen, valbuflen;
//     size_t sector_size = dev->sector_size;

// 	size_t buffer_size = sector_size;
//     void *rbuffer = nvme_alloc(buffer_size,sector_size, &huge);
// 	if (!rbuffer) {
// 		printf("can not allocate io payload\n");
//         return -1;
// 	}
// 	ret = kv_store_read(dev, rbuffer, buffer_size, sector_size, offset);
// 	record_t *keybuf = (record_t *) rbuffer;
// 	if (keybuf->record_len >= sector_size) {
// 		nvme_free(rbuffer, huge);
// 		rbuffer = NULL;
// 		keybuf = NULL;
// 		rbuffer = nvme_alloc(keybuf->record_len, sector_size, &huge);
// 		ret = kv_store_read(dev, rbuffer, buffer_size, sector_size, offset);
// 		keybuf = (record_t *) rbuffer;
// 	}
//     unsigned char *kbuf;
// 	kbuf = calloc(1, keybuf->data_len);
// 	memcpy(kbuf, keybuf->data, keybuf->data_len);

// 	*buf = kbuf;
// 	*buflen = keybuf->data_len;
// 	nvme_free(rbuffer, huge);
// 	return KV_STORE_SUCCESS;
// }

kv_dev_t *kv_store_open(char *dev)
{
	int err, fd;
	int64_t _ssdSizeByte = 0;
	
	struct stat nvme_stat;
	if ((fd = open(dev, O_RDWR | __O_DIRECT)) < 0)
		goto open_err;
	// if((err = fstat(fd, &nvme_stat)) < 0)
	// 	goto stat_err;
	// if (!S_ISCHR(nvme_stat.st_mode) && !S_ISBLK(nvme_stat.st_mode))
	// 	goto dev_err;

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
	int row_size = SD_DATA_ROW_HEAD_SIZE + sizeof(record_t);
	SDataRow row = calloc(1, row_size);
	if(!row) {
		return NULL;
	}
	dataRowSetLen(row, row_size);
  	dataRowSetVersion(row, 1);
	void *data = dataRowTuple(row);
	record_t *buf = (record_t *)data;
	memset(buf, 0, sizeof(record_t));
	buf->watermark = START_MARK;
	buf->record_type = REC_TYPE_KEY;
	buf->record_len = sizeof(record_t);
	buf->data_len = datalen;
	buf->key_len = keylen;
	buf->data_offset = data_offset;
	buf->data_block_id = block_id;
	memcpy(buf->key, key, keylen);
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
	uint32_t block_id;
	if ((check_dev(dev) < 0) || !key || !data || (keylen == 0) || (datalen == 0) ){
		ret = KV_STORE_INVALID_PARAMETER;
		goto err;
	}
	block_id = get_available_block_id(dev->bits, STAR_BLOCK_TYPE_DATA);
	if ((block_id < STAR_DATA_BLOCKID_START) || (block_id > STAR_DATA_BLOCKID_END)) {
		ret = KV_STORE_INVALID_PARAMETER;
		goto err;	
	}
	DataBlock *block = stardb_get_data_block(dev->fd, block_id);
	if(!block){
		ret = KV_STORE_INVALID_PARAMETER;
		goto err;	
	}
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
	ret = mem_put(dev->memtable, rowkey);
	free(rowkey);
	free(block);
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
	SDataRow sdata = mem_get(memtable, key);
	if (!sdata) {
		ret = KV_STORE_ERROR_SKIPLIST;
		__DEBUG(LEVEL_DEBUG, "mem_get %s not exist", key);
		goto err;
	}
    record_t *s_record = (record_t *)dataRowTuple(sdata);
	if (s_record->watermark != START_MARK) {
		ret = KV_STORE_INVALID_KEY;
		goto err;
	}
	DataBlock *get_block = stardb_get_data_block(dev->fd, s_record->data_block_id);
	SDataRow gdata =data_block_get_rowdata(get_block, s_record->data_offset);
	int length = dataRowLen(gdata);
	*buf = calloc(1, length);
	if (!*buf) {
		ret = KV_STORE_INVALID_MALLOC;
		goto err;
	}
	memcpy(*buf, dataRowTuple(gdata), length - SD_DATA_ROW_HEAD_SIZE);
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
err:
	return ret;
}

int kv_store_close(kv_dev_t * dev){
	bit_save(dev->fd, dev->bits);
	if (skipListGetSize(dev->memtable->skipList) > 0)
		mem_flush(dev->fd, dev->memtable);
	return close(dev->fd);
}
