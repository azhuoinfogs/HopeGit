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
}