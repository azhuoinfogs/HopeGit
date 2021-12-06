#include "data_block.h"
int stardb_store_data_block(int fd, DataBlock *dblock)
{
	ssize_t w_bytes = pwrite(fd, (void *)dblock, DATA_BLOCK_SIZE, dblock->block.start_address);
	if ((w_bytes < 0) || (w_bytes != DATA_BLOCK_SIZE)) {
		fprintf(stderr, " can not pwrite data block of device \n");
		return -1;
	}
	return 0;
}

DataBlock *stardb_restore_data_block(int fd, uint32_t block_id)
{
	int err = 0;
	void *buf;
    if (!posix_memalign((void **)&buf, SECTOR_SIZE, DATA_BLOCK_SIZE)) {
        memset(buf, 0, DATA_BLOCK_SIZE);
    }
	uint32_t read_bytes = pread(fd, buf, DATA_BLOCK_SIZE, block_id * DATA_BLOCK_SIZE);
    if(read_bytes != DATA_BLOCK_SIZE) {
		goto errout;
	}
	DataBlock *blk = (DataBlock *)buf;
	if((blk->block.id != block_id) || (blk->block.type != STAR_BLOCK_TYPE_DATA)\
        || (blk->block.size != DATA_BLOCK_SIZE) || (blk->block.start_address != (block_id * DATA_BLOCK_SIZE))\
        || (blk->block.end_address != ((block_id + 1) * DATA_BLOCK_SIZE))){
            goto errout;
    }
	return blk;
errout:
	if (buf)
		free(buf);
	return NULL;
}
DataBlock *stardb_create_data_block(uint32_t block_id)
{
	DataBlock *dblock = aligned_alloc(SECTOR_SIZE, DATA_BLOCK_SIZE);
	if (!dblock) {
		fprintf(stderr, "can not allocate io payload\n");
		return NULL;
	}
	memset(dblock, 0, DATA_BLOCK_SIZE);
	dblock->block.id = block_id;
	dblock->block.is_used = 1;
	dblock->block.size = DATA_BLOCK_SIZE;
	dblock->block.type = STAR_BLOCK_TYPE_DATA;
	dblock->block.used_size = 0;
	dblock->block.free_size = DATA_BLOCK_SIZE;
	dblock->block.start_address = block_id * DATA_BLOCK_SIZE;
	dblock->block.end_address = block_id * DATA_BLOCK_SIZE + DATA_BLOCK_SIZE;
	dblock->max_time = 0;
	dblock->min_time = 0;
	dblock->row_count = 0;
	dblock->max_row_size = MAX_ROW_SIZE;
	dblock->header_size = sizeof(DataBlock);
	dblock->offset = 0;
	return dblock;
}

DataBlock *stardb_get_data_block(int fd, uint32_t block_id)
{   
	bool huge;
	int buffer_size = DATA_BLOCK_SIZE;
	DataBlock *dblock = stardb_restore_data_block(fd, block_id);
	if (!dblock) {
		dblock = stardb_create_data_block(block_id);
	}
	return dblock;
}

void stardb_clear_data_block(int fd, uint32_t block_id, DataBlock *dblock)
{
	memset(dblock, 0, DATA_BLOCK_SIZE);
	dblock->block.id = block_id;
	dblock->block.is_used = 0;
	dblock->block.size = DATA_BLOCK_SIZE;
	dblock->block.type = STAR_BLOCK_TYPE_DATA;
	dblock->block.used_size = 0;
	dblock->block.free_size = DATA_BLOCK_SIZE;
	dblock->block.start_address = block_id * DATA_BLOCK_SIZE;
	dblock->block.end_address = block_id * DATA_BLOCK_SIZE + DATA_BLOCK_SIZE;
	dblock->max_time = 0;
	dblock->min_time = 0;
	dblock->row_count = 0;
	dblock->max_row_size = MAX_ROW_SIZE;
	dblock->header_size = sizeof(DataBlock);
	dblock->offset = 0;
}

void stardb_erase_data_block(int fd, uint32_t block_id, DataBlock *dblock)
{
	memset(dblock, 0, DATA_BLOCK_SIZE);
	dblock->block.id = block_id;
	dblock->block.is_used = 0;
	dblock->block.size = DATA_BLOCK_SIZE;
	dblock->block.type = STAR_BLOCK_TYPE_DATA;
	dblock->block.used_size = 0;
	dblock->block.free_size = DATA_BLOCK_SIZE;
	dblock->block.start_address = block_id * DATA_BLOCK_SIZE;
	dblock->block.end_address = block_id * DATA_BLOCK_SIZE + DATA_BLOCK_SIZE;
	dblock->max_time = 0;
	dblock->min_time = 0;
	dblock->row_count = 0;
	dblock->max_row_size = MAX_ROW_SIZE;
	dblock->header_size = sizeof(DataBlock);
	dblock->offset = 0;
	stardb_store_data_block(fd, dblock);
}
void stardb_free_data_block(DataBlock *block) {
	if (block)
		free(block);
}

size_t data_block_add_rowdata(DataBlock *datablock, SDataRow data)
{
	size_t data_offset = datablock->offset;
	size_t datalen = dataRowLen(data);
	memcpy(datablock->data + datablock->offset, data, datalen);
	datablock->offset +=  datalen;
	datablock->block.used_size += datalen;
	datablock->block.free_size = (datablock->block.size - datablock->block.used_size);
	datablock->row_count++;
	return data_offset;
}

SDataRow data_block_get_rowdata(DataBlock *datablock, size_t offset)
{
	void *data = datablock->data + offset;
	// size_t datalen = dataRowLen(data);
	// SDataRow row = calloc(1, datalen);
	// if(!row) {
	// 	return NULL;
	// }
	// memcpy(row, data, datalen);
	return data;
}

int data_block_add_rowkey(DataBlock *datablock, SDataRow data)
{
	record_t *s_record = (record_t *)dataRowTuple(data);
	int64_t time = 0;
	memcpy(datablock->data + datablock->offset, s_record, s_record->record_len);
	datablock->offset +=  s_record->record_len;
	datablock->block.used_size += s_record->record_len;
	datablock->block.free_size = (datablock->block.size - datablock->block.used_size);
	datablock->row_count++;
	datablock->max_row_size = MAX_ROW_SIZE;
	return 0;
}

int data_block_is_full(struct DataBlock *datablock, uint32_t rowsize)
{
	if ((datablock->offset + rowsize >= datablock->block.size)) {
		return 1;
	}
	return 0;
}

int print_data_block(DataBlock *dblock) {
	printf(" block id:%d size:%d used_size:%d free_size:%d type:%d star_offset:%ld end_offset:%ld\n", dblock->block.id, 
		dblock->block.size, dblock->block.used_size, dblock->block.free_size, dblock->block.type, dblock->block.start_address, dblock->block.end_address);
	printf(" row_count:%d", dblock->row_count);
	printf(" row_size :%d\n", dblock->max_row_size);
	printf(" header_size :%d\n", dblock->header_size);
	printf(" max_time :%ld\n", dblock->max_time);
	printf(" min_time: %ld\n", dblock->min_time);
	printf(" min_key: %s\n", dblock->min_key);
	printf(" max_key: %s\n", dblock->max_key);
	printf(" offset :%d\n", dblock->offset);
	size_t offset = 0;
	for (int i = 0; i < dblock->row_count; i++) {
		record_t *s_record = (record_t *)(dblock->data + offset);
		if (s_record->watermark != START_MARK) {
			break;
		}
		printf("%s--%d\n", s_record->key, s_record->record_len);
		offset +=s_record->record_len;
	}
}

char key_min1[MAX_ROW_SIZE];
char key_min2[MAX_ROW_SIZE];
int block_merge(DataBlock *dblock1, DataBlock *dblock2) {
	DataBlock *dblock3 = stardb_create_data_block(3);
	DataBlock *dblock4 = stardb_create_data_block(4);
	memcpy(key_min1, dblock1->min_key, dblock1->max_row_size);
	memcpy(key_min2, dblock2->min_key, dblock2->max_row_size);
	record_t out_record;
	while(1) {
		int i = 0;
		int j = 0;
		if ((i > dblock1->row_count) || (j > dblock2->row_count)){
			break;
		}
		if (data_block_is_full(dblock3, sizeof(out_record))) {
			break;
		}
		size_t i_offset = 0;
		record_t *s_record = (record_t *)(dblock1->data + i_offset);
		size_t j_offset = 0;
		record_t *t_record = (record_t *)(dblock2->data + j_offset);
		if ((memcmp(s_record->key, key_min1, MAX_ROW_SIZE) < 0) || (memcmp(t_record->key, key_min2, MAX_ROW_SIZE) < 0)) {
			break;
		}
		if (memcmp(s_record->key, key_min2, s_record->key_len) < 0 )
		{
			memcpy(out_record.key, key_min2, strlen(key_min2));
			memcpy(key_min2, s_record->key, s_record->key_len);
			data_block_add_rowdata(dblock3, (SDataRow)&out_record);
		}
		memset(&out_record, 0, sizeof(out_record));
		if (memcmp(t_record->key, key_min1, t_record->key_len) < 0 )
		{
			memcpy(out_record.key, key_min2, strlen(key_min2));
			memcpy(key_min2, t_record->key, t_record->key_len);
			data_block_add_rowdata(dblock3, (SDataRow)&out_record);
		}
		i_offset += s_record->record_len;
		j_offset += t_record->record_len;
	}
}

