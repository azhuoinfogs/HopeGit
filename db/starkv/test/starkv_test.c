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
int main () {
    printf("test kv store\n");
    int ret = 0;
    srand(time(NULL));
    starkv_t *dev  = starkv_open("/dev/nvme0n1");
    starkv_init(dev, 0);
    char key[KSIZE];
	char val[VSIZE];
	char *err = NULL;
	unsigned char *sv;
	size_t val_len;
    int count = 10;
	for (int i = 0; i < count; i++) {
		memset(key, 0, KSIZE);
		memset(val, 0, VSIZE);
        _random_key(key, KSIZE-1);
		_random_key(val, VSIZE-1);
		unsigned char *keybuf =  calloc(1,strlen(key));
		memcpy(keybuf, key, strlen(key));
		size_t keybuf_len = strlen(key);
		unsigned char *valbuf =  calloc(1,strlen(val));
		memcpy(valbuf, val, strlen(val));
		size_t valbuf_len = strlen(val);
		starkv_put(dev, keybuf, keybuf_len, valbuf, valbuf_len, &err);
		ret = starkv_get(dev, keybuf, keybuf_len, &sv, &val_len, &err);
		if(sv) {
			// printf("get:%s--%s--%ld--%ld\n", valbuf, sv, valbuf_len, val_len);
			free(sv);
			sv = NULL;
		} else {
			printf("get:%s  failed\n", keybuf);
		}
		free(keybuf);
		free(valbuf);
	}
    starkv_close(dev);
	starkv_cleanup(dev);
	return ret;
}
