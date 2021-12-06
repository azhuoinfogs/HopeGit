#ifndef _SSDB_BITMAP_H
#define _SSDB_BITMAP_H

#ifdef __cplusplus
extern "C" {
#endif
#include "os.h"
#include "kv_store.h"
/**
 *create a new bitmap
 *@dev nvme disk device name
 *@return return bitmap, all bits set to 0; 
**/
BITS bit_create(int fd);

/**
 *release a bitmap
 *@bit bitmap
 */
void bit_destroy(BITS bit);

/**
 *get current bitmap bits length
 *@bit bitmap
 *@return return bitmap bits length; 
**/
uint32_t bit_length(BITS bit);

/**
 *flush bitmap to ssd block
 *@dev nvme disk device name
 *@bit bitmap
 *@return return saved bitmap bytes length, if return -1, means failed to write; 
**/
uint32_t bit_save(int fd, BITS bit);

/**
 *Get bitmap from name ssd device
 *@dev nvme disk device name
 *@return return a bitmap, if return NULL, means failed to read; 
**/
BITS bit_read(int fd);
/**
 *Erase bitmap in ssd device
 *@dev nvme disk device name
**/
void bit_erase(char *dev);

/**
 *Sets the state of the bit at the specified position in the bitmap
 *@bit bitmap
 *@id block id
 *@status true of false
 *@return -1 failed, 0 success; 
**/
uint32_t bit_set_block_status(BITS bit, uint32_t id, bool status);

/**
 *Sets the state of the bits at the specified position in the bitmap
 *@bit bitmap
 *@start_id start block id
 *@nbit number of set bit
 *@status true of false
 *@return -1 failed, 0 success; 
**/
uint32_t bit_set_nblock_status(BITS bit, uint32_t start_id, uint32_t nbit, bool status);

/**
 *Get the state of the bit at the specified position in the bitmap
 *@bit bitmap
 *@id block id
 *@return status true or false; 
**/
bool bit_get_block_status(BITS bit, uint32_t id);

/**
 *Get the available block id at the specified block type in the bitmap
 *@bit bitmap
 *@type block type
 *@status true of false
 *@return block id, if return -1, means get failed; 
**/
uint32_t get_available_block_id(BITS bit, enum BlockType type);

/**
 *bitmap init
 *@dev nvme disk device name
 *@return -1 failed, 0 success; 
**/
BITS  bitInit(int fd);
void bit_print(BITS starBitmap);
uint32_t bitGetBlockId(BITS starBitmap, enum BlockType type);
uint32_t bitGetNBlockId(BITS starBitmap, enum BlockType type, uint32_t nbit);
uint32_t bitSetNBlockStatus(BITS starBitmap,uint32_t start_id, uint32_t nbit, bool status);
uint32_t bitSetBlockStatus(BITS starBitmap,uint32_t id, bool status);
uint32_t bitGetNBlockId(BITS starBitmap, enum BlockType type, uint32_t nbit);
uint32_t bitGetBlockId(BITS starBitmap, enum BlockType type);
uint32_t bitSave(BITS starBitmap);
bool bitGetBlockStatus(BITS starBitmap,uint32_t id);
uint32_t bitSetNBlockStatus(BITS starBitmap,uint32_t start_id, uint32_t nbit, bool status);
#ifdef __cplusplus
}
#endif

#endif