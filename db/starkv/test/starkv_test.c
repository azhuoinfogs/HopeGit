#include "starkv.h"
#include "../src/skiplist.h"
#include <stdbool.h>
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
int test_case1(){
	char *err;
	size_t num;
	starkv_info_t **infos = starkv_list_dbinfo(&num, &err);
	for (int i = 0; i < num; i++) {
		printf("%s--%s--%d\n", infos[i]->devname, infos[i]->dbname, infos[i]->db_id);
	}
	starkv_array_free((void **)infos, num);
}
int main () {
    printf("test kv store\n");
    int ret = 0;
	char *devname = "/dev/nvme0";

	// int dbindex = starkv_create_database(devname, "test.db", 1073741824, KSIZE, VSIZE);
	// // sleep(1);
	// // int dbindex = 2;
	// starkv_t *dev = starkv_open_database(devname, "test.db", dbindex);
    // char key[KSIZE];
	// char val[VSIZE];
	// char *err = NULL;
	// unsigned char *sv;
	// size_t val_len;
    // int count = 100;
	// for (int i = 0; i < count; i++) {
	// 	memset(key, 0, KSIZE);
	// 	memset(val, 0, VSIZE);
    //     _random_key(key, KSIZE-1);
	// 	_random_key(val, VSIZE-1);
	// 	unsigned char *keybuf =  calloc(1,strlen(key));
	// 	memcpy(keybuf, key, strlen(key));
	// 	size_t keybuf_len = strlen(key);
	// 	unsigned char *valbuf =  calloc(1,strlen(val));
	// 	memcpy(valbuf, val, strlen(val));
	// 	size_t valbuf_len = strlen(val);
	// 	starkv_put(dev, keybuf, keybuf_len, valbuf, valbuf_len, &err);
	// 	if (err) {
	// 		printf("%s\n", err);
	// 		break;
	// 	}
	// 	ret = starkv_get(dev, keybuf, keybuf_len, &sv, &val_len, &err);
	// 	if(sv) {
	// 		if ((memcmp(sv, valbuf, val_len) != 0 ))
	// 			printf("get wrong value:%s--%s\n", sv, valbuf);
	// 		free(sv);
	// 		sv = NULL;
	// 	} else {
	// 		printf("get:%s  failed: %s\n", keybuf, err);
	// 	}
	// 	free(keybuf);
	// 	free(valbuf);
	// }
	// {
	// 	char *ckey ="0djlj";
	// 	starkv_put(dev, ckey, strlen(ckey), ckey, strlen(ckey), &err);
	// 	ret = starkv_get(dev, ckey, strlen(ckey), &sv, &val_len, &err);
	// 	if(sv) {
	// 		if ((memcmp(sv, ckey, val_len) != 0 ))
	// 			printf("get wrong value:%s--%s\n", sv, ckey);
	// 		free(sv);
	// 		sv = NULL;
	// 	} else {
	// 		printf("get:%s  failed\n", ckey);
	// 	}
	// }
	// starkv_iterator_t *sIter = starkv_create_iterator(dev);
	// int scount;
    // while (starkv_iter_next(sIter))
    // {
	// 	size_t key_len;
    //     char *key = starkv_iter_key(sIter, &key_len);
	// 	if (key)
    //     	printf("data:%s--%ld\n", key, key_len);
	// 	size_t val_len;
    //     char *val = starkv_iter_value(sIter, &val_len);
	// 	if (val)
    //     	printf("data:%s--%ld\n", val, val_len);
    //     /* code */
	// 	scount ++;
    // }
	// starkv_iter_destroy(sIter);
    // starkv_close_database(dev);
	// starkv_cleanup_database(dev);
	return ret;
}
