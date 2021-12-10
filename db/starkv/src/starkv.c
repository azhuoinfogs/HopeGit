#include "starkv.h"
#include "kv_store.h"
starkv_t *starkv_open(char *dev) {
    // return (starkv_t*) kv_store_open(dev);
    return NULL;
}
int starkv_init(starkv_t *dev) {
    // kv_dev_t* kdev = (kv_dev_t *)dev;
    // return kv_store_init(kdev, 1);
}
int starkv_close(starkv_t *dev) {
    // kv_dev_t* kdev = (kv_dev_t *)dev;
    // return kv_store_close(kdev);  
}
void starkv_cleanup(starkv_t *dev) {
    // kv_dev_t* kdev = (kv_dev_t *)dev;
    // return kv_store_free(kdev);
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
int starkv_create_database(char *dev, char *dbname, size_t dbsize, int min_key_size, int max_value_size) {
	int ret = KV_STORE_SUCCESS;
    size_t lba_count = dbsize / SECTOR_SIZE;
    int nsid = create_ns(dev, lba_count);
    if (nsid < 0 ) {
        printf("create database failed\n");
        ret = KV_STORE_INVALID_DB;
        goto err;
    }
    sleep(1);
    char devname[32] = {0};
    (void)snprintf(devname, 32, "%sn%d", dev, nsid);
    int ratio = max_value_size / min_key_size;
    kv_dev_t *kdev = kv_create_dev(devname, dbname, nsid);
    if (!kdev) {
        printf("kv_create_dev %s failed\n", devname);
        ret = KV_STORE_INVALID_DEV;
        goto err;
    }
    ret = kv_init_dev(kdev, ratio);
    if (ret != KV_STORE_SUCCESS) {
        printf("kv_store_init %s failed\n", devname);
        goto err;
    }
    kv_close_dev(kdev);
    if(kdev) free(kdev);
    return nsid;
err:
    if(kdev) free(kdev);
    return ret;
}
starkv_t *starkv_open_database(char *dev, char *dbname, int nsid) {
    char devname[32] = {0};
    (void)snprintf(devname, 32, "%sn%d", dev, nsid);
    kv_dev_t *kdev = kv_restore_dev(devname, nsid);
    if (!kdev) {
        printf("restore database %s failed\n", devname);
        goto err;
    }
    return kdev;
err:
    return NULL;
}
int starkv_close_database(starkv_t *dev) {
    kv_dev_t* kdev = (kv_dev_t *)dev;
    return kv_close_dev(kdev);  
}
void starkv_cleanup_database(starkv_t *dev) {
    kv_dev_t* kdev = (kv_dev_t *)dev;
    return kv_destroy_dev(kdev);  
}
void starkv_array_free(void **infoArray, int num) {
    for (size_t i = 0; i < num; i++) {
        if (infoArray[i])
            free(infoArray[i]);
    }
    if (infoArray)
        free(infoArray);
}
starkv_info_t **starkv_list_column_families(size_t* lencfs, char** errptr) {
    int num;
    int fd;
    starkv_info_t **infos = NULL;
    char **name = list_ns(&num);
    if((!name) || (num == 0)) {
		goto err;
    }
    infos = malloc(num * sizeof(starkv_info_t *));
    if (!infos) {
		goto err;
    }
    for (int i = 0; i < num; i++) {
        infos[i] = calloc(1, sizeof(starkv_info_t));
        if (!infos[i]) {
		    goto err;
        }
        int fd;
	    if ((fd = open(name[i], O_RDWR | __O_DIRECT)) < 0) {
		    goto err;
	    }
        dev_header_t *header = kv_restore_devheader(fd);
        if (!header) {
		    goto err;
        }
        infos[i]->db_id = header->db_id;
        strcpy(infos[i]->dbname, header->dbname);
        strcpy(infos[i]->devname, header->devname);
        if (header) free(header);
        if (fd) close(fd);
    }
    *lencfs = num;
    if (name) starkv_array_free((void **)name, MAX_DEVICE_COUNT);
    return infos;
err:
    if (name) starkv_array_free((void **)name, MAX_DEVICE_COUNT);
    if (infos) starkv_array_free((void **)infos, num);
    return NULL;
}