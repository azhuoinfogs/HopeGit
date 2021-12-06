#include "../src/mem.h"
#define KSIZE	16
#define VSIZE	80
void _random_key(char* key, int length) 
{
	char salt[36] = "abcdefghijklmnopqrstuvwxyz0123456789";
	int i;
	memset(key, 0, length);
	for(i = 0; i < length; i++)
		key[i] = salt[rand() % 36];
}

int main() {
    printf("test mem\n");
    srand(time(NULL));
    char key[KSIZE];
	char val[VSIZE];
    int count =10;
    int fd;
    if ((fd = open(DEV_NAME, O_RDWR | __O_DIRECT)) < 0) {
        printf("open failed");
        return -1;
    }
    // struct mem *mem = mem_create(5200);
    // uint32_t block_id = 5200;
    // for (int i = 0; i < count; i++) {
    //     _random_key(key, KSIZE-1);
	// 	_random_key(val, VSIZE-1);
	// 	unsigned char *keybuf =  calloc(1,strlen(key));
	// 	memcpy(keybuf, key, strlen(key));
	// 	size_t keybuf_len = strlen(key);
	// 	unsigned char *valbuf =  calloc(1,strlen(val));
	// 	memcpy(valbuf, val, strlen(val));
	// 	size_t valbuf_len = strlen(val);

    //     SDataRow row = kv_prepare( keybuf, keybuf_len, valbuf, valbuf_len);
    //     mem_put(mem, row);
    //     free(keybuf);
    //     free(valbuf);
	// }
    // {
    //     _random_key(key, KSIZE-1);
	// 	_random_key(val, VSIZE-1);
	// 	unsigned char *keybuf =  calloc(1,strlen(key));
	// 	memcpy(keybuf, key, strlen(key));
	// 	size_t keybuf_len = strlen(key);
	// 	unsigned char *valbuf =  calloc(1,strlen(val));
	// 	memcpy(valbuf, val, strlen(val));
	// 	size_t valbuf_len = strlen(val);
    //     SDataRow row = kv_prepare( keybuf, keybuf_len, valbuf, valbuf_len);
    //     mem_print(mem);
    //     SDataRow prow = mem_put_and_pop_max(mem, row);
	// 	record_t *s_record = (record_t *)dataRowTuple(prow);
    //     printf("pop max-->%s\n", s_record->key);
    //     mem_print(mem);

    // }
    // struct mem *rmem  = mem_restore(fd, 5200);
    // mem_print(rmem);
	// mem_clear(rmem);
    // mem_print(rmem);


  // NOTE: do not allocate the space for key, since in each skip list node, only keep the pointer to pTable, not the
  // actual key value, and the key value will be retrieved during query through the pTable and getTagIndexKey function

    // printf("%ld--%ld--%ld\n",db_skipList->state.nTotalMemSize, db_skipList->state.nInsertObjs, skipListGetSize(db_skipList));

    // SSkipListIterator *sIter = skipListCreateIter(db_skipList);
    // while (skipListIterNext(sIter))
    // {
    //     SSkipListNode *node = skipListIterGet(sIter);
    //     key_record_t *data = SL_GET_NODE_DATA(node);
    //     printf("xxxx:%s--%ld\n", data->data, data->value_offset);
    //     /* code */
    // }
}
