#include "starkv.h"
#include "../src/os.h"
#include "../src/util.h"
int main() {
    printf("test util\n");
    int fd;
    if ((fd = open("/dev/nvme0n5", O_RDWR | __O_DIRECT)) < 0) {
        printf("open failed");
        return -1;
    }
    // char res[] = "create-ns: Success, created nsid:3";
    // int i =0;
    // int found = 0;
    // while (res[i] != '\0') {
    //     if (res[i] >= '0' && res[i] <= '9') {
    //         found = 1;
    //         break;
    //     }
    //     i++;
    // }
    // if (found)
    //     printf("%d\n" , res[i] - '0');
}