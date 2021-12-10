#include "../src/data_block.h"
#include "../src/skiplist.h"
#include "../src/kv_store.h"
#include "starkv.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
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
    char *devname = "/dev/nvme0";
    int dbindex = starkv_create_database(devname, "test.db", 1073741824, 1, 10);
    char dev[32] = {0};
    // int dbindex = 8;
    (void)snprintf(dev, 32, "%sn%d", devname, dbindex);
    int fd;
    if ((fd = open(dev, O_RDWR | __O_DIRECT)) < 0) {
        printf("open failed:%s\n", DEV_NAME);
        return -1;
    }
    dev_header_t header;
    header.bit_blockid_end = 1;
    header.bit_blockid_start = 1;
    header.mem_blockid_end = 1;
    header.mem_blockid_start = 1;
    header.data_blockid_end = 1;
    header.data_blockid_start = 1;
    strcpy(header.devname, dev);
    strcpy(header.dbname, "test.db");
    header.db_id = dbindex;
    DataBlock *datablock1 = stardb_create_data_block(0);
    if (!datablock1) {
        printf("stardb_get_data_block failed\n");
    }
    memcpy(datablock1->data, &header, sizeof(dev_header_t));
    // stardb_store_data_block(fd, datablock1);
    // free(datablock1);
    // close(fd);
    // int fd2;
    // if ((fd2 = open(devname, O_RDWR | __O_DIRECT)) < 0) {
    //     printf("open failed:%s\n", DEV_NAME);
    //     return -1;
    // }
    DataBlock *datablock2 = stardb_restore_data_block(fd, 0);
    dev_header_t header1;
    memcpy(&header1, datablock2->data, sizeof(dev_header_t));
    printf("%d--%s\n", header1.db_id, header1.devname);
    free(datablock2);
    // print_data_block(datablock1);
    // print_data_block(datablock2);
}