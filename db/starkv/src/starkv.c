#include "starkv.h"
#include "kv_store.h"
starkv_t *starkv_open(char *dev) {
    return (starkv_t*) kv_store_open(dev);
}
int starkv_init(starkv_t *dev) {
    kv_dev_t* kdev = (kv_dev_t *)dev;
    return kv_store_init(kdev);
}
int starkv_close(starkv_t *dev) {
    kv_dev_t* kdev = (kv_dev_t *)dev;
    return kv_store_close(kdev);  
}
void starkv_cleanup(starkv_t *dev) {
    kv_dev_t* kdev = (kv_dev_t *)dev;
    return kv_store_free(kdev);
}
int starkv_put(starkv_t *dev, unsigned char *key, size_t keylen,unsigned char *data, size_t datalen, char **errptr) {
    kv_dev_t* kdev = (kv_dev_t *)dev;
    return kv_store_put(kdev, key, keylen, data, datalen, errptr);
}
int starkv_get(starkv_t *dev, unsigned char *key, size_t keylen, unsigned char **buf, size_t *buflen, char **errptr) {
    kv_dev_t* kdev = (kv_dev_t *)dev;
    return kv_store_get(kdev, key, keylen, buf, buflen, errptr);
}
starkv_iterator_t* starkv_create_iterator(starkv_t* dev) {
    kv_dev_t* kdev = (kv_dev_t *)dev;
    return (starkv_iterator_t* )kv_create_iter(kdev);
 }
void starkv_iter_destroy(starkv_iterator_t* iter) {
    kv_destroy_iter(iter);
    return;
}
bool starkv_iter_next(starkv_iterator_t* iter) {
    return kv_iter_next(iter);
}
char* starkv_iter_key(void* iter, size_t* klen) {
    return kv_iter_key(iter, klen);
}
char* starkv_iter_value(void* iter, size_t* klen) {
    return kv_iter_value(iter, klen);
}