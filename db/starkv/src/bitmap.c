#include "os.h"
#include "bitmap.h"
#include <linux/fs.h>
BITS bit_create(int fd)
{
	int64_t ssdSizeBytes;
	int res = ioctl(fd, BLKGETSIZE64, &ssdSizeBytes);
	if (res < 0) {        
		goto err;
	}

    uint32_t bit_block_nums = ssdSizeBytes / (BITMAP_BLOCK_SIZE);
    uint32_t bit_char_nums = sizeof(char) * (bit_block_nums >> 3) + 1;

    BITS bmp  = (BITS)malloc(sizeof(Bits) + bit_char_nums);
    pthread_mutex_init(&bmp->mutex, NULL);
    if (!bmp){
        goto err;
    }

    bmp->num_of_bytes = bit_char_nums;
    bmp->num_of_bits = bit_block_nums;  
    memset(bmp->bits, 0, bit_char_nums);
    // for (size_t i = 0; i < bit_char_nums; i++){
    //     bmp->bits[i] = i % 256;
    // }
    return bmp;
err:
    return NULL;
}

void bit_destroy(BITS bit)
{
    free(bit);
}

uint32_t bit_length(BITS bit)
{
    uint32_t num_of_bits;
    pthread_mutex_lock(&bit->mutex);
    num_of_bits = bit->num_of_bits;

    pthread_mutex_unlock(&bit->mutex);
    return num_of_bits;
}

uint32_t bit_save(int fd, BITS bit)
{
    if(!bit){
        return -1;
    }
    pthread_mutex_lock(&bit->mutex);
    uint32_t data_size = sizeof(Bits) + bit->num_of_bytes;
    uint32_t max_datalen_each_block = BITMAP_BLOCK_SIZE - sizeof(BitBlock);
    uint32_t block_nums = data_size / max_datalen_each_block + (((data_size % max_datalen_each_block) == 0) ? 0 : 1);
    uint32_t total_block_size = block_nums * BITMAP_BLOCK_SIZE;
    void *buf;
    if (!posix_memalign((void **)&buf, getpagesize(), total_block_size)) {
		memset(buf, 0, total_block_size);
	}
    else{
        printf("alloc posix_memalign feild\n");
        pthread_mutex_unlock(&bit->mutex);
        goto err;
    }

    pthread_mutex_unlock(&bit->mutex);
    bit_set_nblock_status(bit, STAR_BITMAP_BLOCKID_START, block_nums, true);
    pthread_mutex_lock(&bit->mutex);
    BitBlock *blk = (BitBlock *)malloc(BITMAP_BLOCK_SIZE);
    for (int i = STAR_BITMAP_BLOCKID_START; i < block_nums; i++){
        memset(blk, 0, BITMAP_BLOCK_SIZE);
        blk->block.id = i;
        blk->block.size = BITMAP_BLOCK_SIZE;
        blk->block.type = STAR_BLOCK_TYPE_BITMAP;
        blk->block.start_address = i * BITMAP_BLOCK_SIZE;
        blk->block.end_address = BITMAP_BLOCK_SIZE * (i + 1);
        uint32_t cur_block_data_size = (((i+1) * max_datalen_each_block) > data_size) ? (data_size % max_datalen_each_block) : max_datalen_each_block;
        
        blk->block.used_size = cur_block_data_size + sizeof(BitBlock);
        blk->block.free_size = BITMAP_BLOCK_SIZE - sizeof(BitBlock) - cur_block_data_size;
        blk->bitLength = cur_block_data_size;
        memcpy(blk->data, (char *)bit + i * max_datalen_each_block, cur_block_data_size);
        memcpy((char *)buf + blk->block.start_address, (char *)blk, BITMAP_BLOCK_SIZE);
    }
    
    if(pwrite(fd, buf, total_block_size, 0) <= 0){
        printf("Write block to ssd feild\n");
        pthread_mutex_unlock(&bit->mutex);
        free(blk);
        goto err;
    }
    pthread_mutex_unlock(&bit->mutex);
    free(blk);
    free(buf);
    return data_size;
err:
    return -1;
}

BITS bit_read(int fd)
{
    BITS bit;
    bit = (Bits *)malloc(sizeof(Bits));
    if(!bit) goto err;
    void *buf;
    if (!posix_memalign((void **)&buf, getpagesize(), BITMAP_BLOCK_SIZE)) {
        memset(buf, 0, BITMAP_BLOCK_SIZE);
    }
    else{
        printf("alloc posix_memalign feild\n");
        goto err;
    }
    int total_data_size = 0, data_offset = 0;
    for (size_t block_id = STAR_BITMAP_BLOCKID_START; block_id < (STAR_BITMAP_BLOCKID_END + 1); block_id++){
        memset(buf, 0, BITMAP_BLOCK_SIZE);       
        uint32_t read_bytes = pread(fd, buf, BITMAP_BLOCK_SIZE, block_id * BITMAP_BLOCK_SIZE);
        if(read_bytes != BITMAP_BLOCK_SIZE) { free(bit); free(buf); goto err; }
        BitBlock *blk = (BitBlock *)buf;
        if((blk->block.id != block_id) || (blk->block.type != STAR_BLOCK_TYPE_BITMAP)\
        || (blk->block.size != BITMAP_BLOCK_SIZE) || (blk->block.start_address != (block_id * BITMAP_BLOCK_SIZE))\
        || (blk->block.end_address != ((block_id + 1) * BITMAP_BLOCK_SIZE)) || (blk->block.used_size == 0)){
            break;
        }
        
        total_data_size += blk->bitLength;
        void *temp = realloc(bit, total_data_size);
        if(!temp) { free(bit); free(buf); goto err; }
        bit = (Bits *)temp;
        memcpy((char *) bit + data_offset, blk->data, blk->bitLength);
        data_offset = total_data_size;        
    }
    pthread_mutex_init(&bit->mutex, NULL);
    free(buf);
    if((total_data_size != (sizeof(Bits) + bit->num_of_bytes)) || (total_data_size == 0)){
        free(bit);
        bit = NULL;
    }
   
    return bit;
err:
    return NULL;
}

void bit_erase(char *dev)
{
	int fd = open(dev, O_RDWR | __O_DIRECT);
	if (fd < 0) {
		goto err;
	}

    void *buf;
    if (!posix_memalign((void **)&buf, getpagesize(), BITMAP_BLOCK_SIZE)) {
        memset(buf, 0, BITMAP_BLOCK_SIZE);
    }
    else{
        printf("alloc posix_memalign feild\n");
        goto err;
    }

    for (size_t block_id = STAR_BITMAP_BLOCKID_START; block_id < (STAR_BITMAP_BLOCKID_END + 1); block_id++){
        if(pwrite(fd, buf, BITMAP_BLOCK_SIZE, block_id * BITMAP_BLOCK_SIZE) <= 0){
            fprintf(stderr, "Erase block in ssd feild\n");
            free(buf);
            goto err;
        }
    }
    free(buf);
err:
    close(fd);
}

uint32_t bit_set_block_status(BITS bit, uint32_t id, bool status)
{
    pthread_mutex_lock(&bit->mutex);
    if(id > bit->num_of_bits) {
        pthread_mutex_unlock(&bit->mutex);
        return -1;
    }
    u_char mask = 0x80 >> (id & 0x7);
    if(status) {
        bit->bits[id >> 3] |= mask;
    } else {
        bit->bits[id >> 3] &= ~mask;
    }
    pthread_mutex_unlock(&bit->mutex);
    return 0;
}

uint32_t bit_set_nblock_status(BITS bit, uint32_t start_id, uint32_t nbit, bool status)
{
    pthread_mutex_lock(&bit->mutex);
    for (uint32_t i = 0; i < nbit; i++){
        uint32_t pos = start_id + i;
        if(pos > bit->num_of_bits) {
            pthread_mutex_unlock(&bit->mutex);
            return -1;
        }
        u_char mask = 0x80 >> (pos & 0x7);
        if(status) {
            bit->bits[pos >> 3] |= mask;
        } else {
            bit->bits[pos >> 3] &= ~mask;
        }
    }
    pthread_mutex_unlock(&bit->mutex);
    return 0;
}

bool bit_get_block_status(BITS bit, uint32_t id)
{
    u_char mask = 0x80 >> (id & 0x7);
    pthread_mutex_lock(&bit->mutex);
    bool state = ((mask & bit->bits[id >> 3]) == mask) ? true : false;
    pthread_mutex_unlock(&bit->mutex);
    return state;
}

uint32_t get_available_block_id(BITS bit, enum BlockType type)
{
    uint32_t res_id = -1;
    switch (type)
    {
    case STAR_BLOCK_TYPE_BITMAP:
        for (size_t i = STAR_BITMAP_BLOCKID_START; i < STAR_BITMAP_BLOCKID_END; i++){
            if(!bit_get_block_status(bit, i)){
                res_id = i;
                break;
            }
        }
        break;
    
    case STAR_BLOCK_TYPE_WAL:
        for (size_t i = STAR_WAL_BLOCKID_START; i < STAR_WAL_BLOCKID_END; i++){
            if(!bit_get_block_status(bit, i)){
                res_id = i;
                break;
            }
        }
        break;

    case STAR_BLOCK_TYPE_DBINFO:
        for (size_t i = STAR_DBINFO_BLOCKID_START; i < STAR_DBINFO_BLOCKID_END; i++){
            if(!bit_get_block_status(bit, i)){
                res_id = i;
                break;
            }
        }
        break;
    
    case STAR_BLOCK_TYPE_TBINFO:
        for (size_t i = STAR_TBINFO_BLOCKID_START; i < STAR_TBINFO_BLOCKID_END; i++){
            if(!bit_get_block_status(bit, i)){
                res_id = i;
                break;
            }
        }
        break;

    case STAR_BLOCK_TYPE_DATA:
        for (size_t i = STAR_DATA_BLOCKID_START; i < bit->num_of_bits; i++){
            if(!bit_get_block_status(bit, i)){
                res_id = i;
                break;
            }
        }
        break;
    
    default:
        fprintf(stderr, "unknown block type!\n");
        break;
    }
    return res_id;
}

uint32_t get_available_block_list(BITS bit, enum BlockType type, uint32_t nbit)
{
    uint32_t res_id = -1;
    uint32_t nilCnt = 0;
    uint32_t start = STAR_DATA_BLOCKID_START;
    switch (type)
    {
    case STAR_BLOCK_TYPE_DATA:
        while (1){
            nilCnt = 0;
            for (size_t i = start; i < bit->num_of_bits; i++){
                if(bit_get_block_status(bit, i)){
                    start = i + 1;
                    break;
                }
                nilCnt ++;
                if(nilCnt >= nbit) {
                    res_id = start;
                    break;
                }
            }
            if(nilCnt >= nbit) break;
        }
        break;
    
    default:
        fprintf(stderr, "error block type!\n");
        break;
    }
    return res_id;
}

BITS  bitInit(int fd)
{
    BITS starBitmap = bit_read(fd);
    if(!starBitmap){
        fprintf(stdout, "Create new bitmap\n");
        starBitmap = bit_create(fd);
        if(!starBitmap){
            fprintf(stdout, "bit map create failed\n");
            return NULL;
        }
        printf("bitMap created, num_of_bits %d\n", starBitmap->num_of_bits);

        int res = bit_save(fd, starBitmap);
        if(res > 0) {
            printf("bitMap saved, size %d\n", res);
        }
        else 
        { 
            printf("bitMap save failed, res %d\n", res); 
            return NULL; 
        }        
    }
    return starBitmap;
}

uint32_t bitGetBlockId(BITS starBitmap, enum BlockType type)
{
   return get_available_block_id(starBitmap, type);
}

uint32_t bitGetNBlockId(BITS starBitmap, enum BlockType type, uint32_t nbit)
{
   return get_available_block_list(starBitmap, type, nbit);
}

uint32_t bitSetBlockStatus(BITS starBitmap,uint32_t id, bool status)
{
    return bit_set_block_status(starBitmap, id, status);
}

uint32_t bitSetNBlockStatus(BITS starBitmap,uint32_t start_id, uint32_t nbit, bool status)
{
    return bit_set_nblock_status(starBitmap, start_id, nbit, status);
}

bool bitGetBlockStatus(BITS starBitmap,uint32_t id)
{
    return bit_get_block_status(starBitmap, id);
}

// uint32_t bitSave(BITS starBitmap)
// {
//     return bit_save(DEV_NAME, starBitmap);
// }
void bit_print(BITS starBitmap) {
    printf("free_blocks:%d\n", starBitmap->free_blocks);
    printf("used_blocks:%d\n", starBitmap->used_blocks);
    printf("total_blocks:%d\n", starBitmap->total_blocks);
    printf("num_of_bits:%d\n", starBitmap->num_of_bits);
    printf("num_of_bytes:%d\n", starBitmap->num_of_bytes);
}