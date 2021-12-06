#ifndef _SSDB_DATA_BLOCK_H
#define _SSDB_DATA_BLOCK_H
#ifdef __cplusplus
extern "C" {
#endif
#include "os.h"
typedef void *SDataRow;
#define MAX_ROW_SIZE 1024
#define DATA_BLOCK_SIZE (128 * 1024)
extern int stardb_store_data_block(int fd, struct DataBlock *dblock);
extern int stardb_read_data_block(int fd, struct DataBlock *dblock);
extern DataBlock *stardb_create_data_block(uint32_t block_id);
extern uint32_t get_data_block_id();
extern void stardb_free_data_block(DataBlock *block);
extern DataBlock *stardb_restore_data_block(int fd, uint32_t block_id);
extern DataBlock *stardb_get_data_block(int fd, uint32_t block_id);
extern size_t data_block_add_rowdata( DataBlock *datablock, SDataRow data);
extern int data_block_add_rowkey(DataBlock *datablock, SDataRow data);
extern int data_block_is_full(struct DataBlock *datablock, uint32_t rowsize);
extern void stardb_clear_data_block(int fd, uint32_t block_id, DataBlock *dblock);
extern int print_data_block(DataBlock *dblock);
extern SDataRow data_block_get_rowdata(DataBlock *datablock, size_t offset);
extern void stardb_erase_data_block(int fd, uint32_t block_id, DataBlock *dblock);
#ifdef __cplusplus
}
#endif

#endif