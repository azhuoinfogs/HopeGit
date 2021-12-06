#include "../src/meta.h"
#include "../src/os.h"
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#define KSIZE 32

int main() {
    printf ("test meta\n");
    int fd;
    if ((fd = open(DEV_NAME, O_RDWR | __O_DIRECT)) < 0) {
        printf("open failed");
        return -1;
    }
    struct meta *meta = meta_new();
    char key[KSIZE];
    char val[KSIZE];
    for (int i = 0; i < 100; i++) {
        snprintf(key, KSIZE, "key:%d", i);
        snprintf(val, KSIZE, "val:%d", i);
        struct meta_node *node = calloc(1, sizeof(struct meta_node));
        strcpy(node->end, key);
        node->count = 100;
        node->block_id = i;
        node->lsn = 123345;
        meta_set(meta, node);
        free(node);
    }
    struct meta_node *node = meta_get(meta, "key:70");
    printf("%d\n", node->block_id);
    meta_flush(fd, meta);
    struct meta *meta1 =  meta_restore(fd);
    struct meta_node *node1 = meta_get(meta, "l");
    
    printf("%d\n", node1->block_id);
}