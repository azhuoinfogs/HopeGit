#include "../src/skiplist.h"
#include "../src/kv_store.h"
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
void _random_key(char* key, int length) 
{
	char salt[36] = "abcdefghijklmnopqrstuvwxyz0123456789";
	int i;
	memset(key, 0, length);
	for(i = 0; i < length; i++)
		key[i] = salt[rand() % 36];
}

static char *sdbGetTsTupleKey(const void *data) { return dataRowTuple(data); }

/* A data row, the format is like below:
 * |<--------------------+--------------------------- len ---------------------------------->|
 * |<--     Head      -->|<---------   klen -------------->|<---------   vlen -------------->|                                 |
 * +---------------------+---------------------------------+---------------------------------+
 * | uint16_t |  int16_t |                                 |                                 |
 * +----------+----------+---------------------------------+---------------------------------+
 * |   len    | sversion |             key part            |             value index         |
 * +----------+----------+---------------------------------+---------------------------------+
 */
int main() {
    printf("test skiplist\n");
    char      *pWrite;
    SSkipList *db_skipList = NULL;
    if (db_skipList == NULL)
		db_skipList = skipListCreate(5, STAR_DATA_TYPE_BINARY, sizeof(int32_t), 0, 0, true, sdbGetTsTupleKey);
	if (db_skipList == NULL) {
		return -1;
	}
    char key[KSIZE];
	char val[VSIZE];
    int count = 10;
    for (int i = 0; i < count; i++) {
        memset(key, 0, KSIZE);
        memset(val, 0, VSIZE);
        _random_key(key, KSIZE-1);
		_random_key(val, VSIZE-1);
        v_index v;
		tstr *keybuf =  calloc(1, sizeof(VarDataLenT) + strlen(key));
        v.value_block_id = 0;
        v.value_offset = i;
        v.value_len = i*10;

        STR_TO_VARSTR(keybuf, key);

        int32_t level = 0;
        int32_t headSize = 0;
        int size = SD_DATA_ROW_HEAD_SIZE + varDataTLen(keybuf) + sizeof(struct v_index);

        SDataRow row = malloc(size);
        if (row == NULL) return -1;
        memset(row, 0, size);
        dataRowSetLen(row, size);
        dataRowSetVersion(row, 1);
        void *sdata = dataRowTuple(row);
        memcpy(sdata, keybuf, varDataTLen(keybuf));
        memcpy(sdata + varDataTLen(keybuf), &v, sizeof(struct v_index));

        skipListNewNodeInfo(db_skipList, &level, &headSize);
        
        size_t bytes = headSize + dataRowLen(row) + sizeof(struct v_index);
        SSkipListNode *pNode = calloc(1, bytes);
        if (pNode == NULL) {
            return -1;
        }
        pNode->level = level;
        dataRowCpy(SL_GET_NODE_DATA(pNode), row);
        skipListPut(db_skipList, pNode);

        SArray *sa = skipListGet(db_skipList, (SSkipListKey)keybuf);
        if (sa->size >= 0) {
        	SSkipListNode *pNode = (SSkipListNode *)starArrayGetP(sa, 0);
            void *sdata= SL_GET_NODE_DATA(pNode);
            void *gdata = dataRowTuple(sdata);
            tstr *da = (tstr*) gdata;
            struct v_index * dv = (struct v_index *)(gdata + varDataTLen(gdata));
            printf("%s-%d-%d--%d--%d\n" , da->data, da->len, dv->value_len, dv->value_offset,  dataRowLen(sdata));
        }
		free(keybuf);
	}
    // char *max_key =  SL_GET_SL_MAX_KEY(db_skipList);
    // printf("%s\n", max_key);
    // SSkipListIterator *sIter = skipListCreateIter(db_skipList);
	// printf("count:%ld\n", skipListGetSize(db_skipList));
    // while (skipListIterNext(sIter))
    // {
    //     SSkipListNode *node = skipListIterGet(sIter);
	// 	char *sdata = SL_GET_NODE_DATA(node);
    //     printf("skiplist->data:%s\n", sdata);
    //     /* code */
    // }

}
