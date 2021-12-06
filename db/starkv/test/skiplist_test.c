#include "../src/skiplist.h"
#include "../src/util.h"
#define MAX_DB_NAME_LEN 32
#define MAX_TABLE_LEN   4096
#define MAX_DB_COUNT    100
#define MAX_BLOCK_COUNT    4096
#define MAX_TABLE_COUNT    1024

#define KSIZE	16
#define VSIZE	80
typedef struct key_record{
     __u64 watermark;
    int record_type;
    int record_len;
    size_t data_len;
    size_t value_len;
    size_t value_offset;
    char data[];
}key_record_t;
char *getkey(const void *data)
{
	return (char *)data;
}
void _random_key(char* key, int length) 
{
	char salt[36] = "abcdefghijklmnopqrstuvwxyz0123456789";
	int i;
	memset(key, 0, length);
	for(i = 0; i < length; i++)
		key[i] = salt[rand() % 36];
}
int main() {
    printf("test skiplist\n");
    SSkipList *db_skipList = NULL;
    if (db_skipList == NULL)
		db_skipList = skipListCreate(100, STAR_DATA_TYPE_BINARY, sizeof(int32_t), 0, false, true, getkey);
	if (db_skipList == NULL) {
		return -1;
	}
    char key[KSIZE];
	char val[VSIZE];
    int count = 10;
    for (int i = 0; i < count; i++) {
        _random_key(key, KSIZE-1);
		_random_key(val, VSIZE-1);
		unsigned char *keybuf =  calloc(1,strlen(key));
		memcpy(keybuf, key, strlen(key));
		size_t keybuf_len = strlen(key);
		unsigned char *valbuf =  calloc(1,strlen(val));
		memcpy(valbuf, val, strlen(val));
		size_t valbuf_len = strlen(val);
    
        int32_t level = 0;
        int32_t headSize = 0;
        skipListNewNodeInfo(db_skipList, &level, &headSize);
        SSkipListNode *pNode = calloc(1, headSize + keybuf_len);
        if (pNode == NULL) {
            return -1;
        }
        pNode->level = level;
        memcpy(SL_GET_NODE_DATA(pNode), keybuf,  keybuf_len);
        skipListPut(db_skipList, pNode);
		free(keybuf);
		free(valbuf);
	}
    char *max_key =  SL_GET_SL_MAX_KEY(db_skipList);
    printf("%s\n", max_key);
    SSkipListIterator *sIter = skipListCreateIter(db_skipList);
	printf("count:%ld\n", skipListGetSize(db_skipList));
    while (skipListIterNext(sIter))
    {
        SSkipListNode *node = skipListIterGet(sIter);
		char *sdata = SL_GET_NODE_DATA(node);
        printf("skiplist->data:%s\n", sdata);
        /* code */
    }

}
