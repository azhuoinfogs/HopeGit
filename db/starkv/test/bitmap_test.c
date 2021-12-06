#include "../src/bitmap.h"
#include<stdlib.h>
#include<stdio.h>
int main() {
    printf ("test bitblock\n");
    int fd;
    if ((fd = open(DEV_NAME, O_RDWR | __O_DIRECT)) < 0) {
        printf("open failed");
        return -1;
    }
    BITS bits = bitInit(fd);
    int len = bit_length(bits);
    int block_id = bitGetBlockId(bits, STAR_BLOCK_TYPE_DATA);
    printf("%d\n", block_id);
    bit_set_block_status(bits, block_id, true);
    int block_id1 = bitGetBlockId(bits, STAR_BLOCK_TYPE_DATA);
    printf("%d\n", block_id1);
    bit_set_block_status(bits, block_id1, true);
    bit_save(fd, bits);
}