#include "../src/data_block.h"
#include "../src/skiplist.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#define DEV_NAME "/dev/nvme0n1"
void _random_key(char* key, int length) 
{
	char salt[36] = "abcdefghijklmnopqrstuvwxyz0123456789";
	int i;
	memset(key, 0, length);
	for(i = 0; i < length; i++)
		key[i] = salt[rand() % 36];
}
char *getkey(const void *data)
{
	return (char *)data;
}

int main () {
    printf("test datablock \n");
    srand(time(NULL));
    int fd;
    if ((fd = open(DEV_NAME, O_RDWR | __O_DIRECT)) < 0) {
        printf("open failed:%s\n", DEV_NAME);
        return -1;
    }
    SSkipList *db_skipList = NULL;
    if (db_skipList == NULL)
		db_skipList = skipListCreate(100, STAR_DATA_TYPE_BINARY, sizeof(int32_t), 0, false, true, getkey);
	if (db_skipList == NULL) {
		return -1;
	}

    // stardb_clear_data_block(fd, 0, datablock1);
    DataBlock *datablock1 = stardb_create_data_block(1);
    if (!datablock1) {
        printf("stardb_get_data_block failed\n");
    }
    DataBlock *datablock2 = stardb_create_data_block(2);
    if (!datablock2) {
        printf("stardb_get_data_block failed\n");
    }
    // stardb_clear_data_block(fd, 0, datablock1);
    char key[31];
    while(1) {
        _random_key(key, 31);
        record_t record;
        record.watermark = START_MARK;
        record.data_block_id = 1;
        record.record_type = 0;
        record.record_len = sizeof(record);
        record.key_len = strlen(key);
        record.data_len = 0;
        record.data_offset = 0;
        strcpy(record.key, key);
        if (data_block_is_full(datablock1, sizeof(record_t))) {
            printf("block is full\n");
            // stardb_store_data_block(fd, datablock1);
            break;
        }
        data_block_add_rowdata(datablock1, (SDataRow)&record);
        memset(key, 0, 31);
        memset(&record, 0, sizeof(record));
    }
    while(1) {
        _random_key(key, 31);
        record_t record;
        record.watermark = START_MARK;
        record.data_block_id = 2;
        record.record_type = 0;
        record.record_len = sizeof(record);
        record.key_len = strlen(key);
        record.data_len = 0;
        record.data_offset = 0;
        strcpy(record.key, key);
        if (data_block_is_full(datablock2, sizeof(record_t))) {
            printf("block is full\n");
            // stardb_store_data_block(fd, datablock1);
            break;
        }
        data_block_add_rowdata(datablock2, (SDataRow)&record);
        memset(key, 0, 31);
        memset(&record, 0, sizeof(record));
        
    }
    // print_data_block(datablock1);
    // print_data_block(datablock2);
}