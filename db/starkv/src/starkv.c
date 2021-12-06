#include "starkv.h"
#include "kv_store.h"
starkv_t *starkv_open(char *dev) {
    return (starkv_t*) kv_store_open(dev);
}
int starkv_init(starkv_t *dev, int create_as_new) {
    kv_dev_t* kdev = (kv_dev_t *)dev;
    return kv_store_init(kdev, create_as_new);
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
