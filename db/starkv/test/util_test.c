#include "starkv.h"
#include "../src/os.h"
#include "../src/util.h"
void nameArrayFree(char **infoArray) {
	char **arrayItself = infoArray;
	char *p = *infoArray;
	while (p) {
		free(p);
		infoArray++;
		p = *infoArray;
	}
	free(arrayItself);
}	
int main() {
    printf("test util\n");
    int num;
    char **name = list_ns(&num);
    for (int i = 0; i < num; i++) {
        printf("%s--%d\n", name[i], num);
    }
    nameArrayFree(name);
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