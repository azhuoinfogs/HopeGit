#include "starkv_store_impl.h"
namespace kv_store {
LdbStore::LdbStore(const char* dev ){
    option_ = starkv_options_create(1073741824, 10, 100);
	starkv_info_t **infos = (starkv_info_t **)starkv_list_dbinfo(&db_num, &err);
    if (db_num == 0 ){
        starkv_create_database(const_cast<char *>(dev), (char*)"default", option_);
    }
    starkv_array_free((void **)infos, db_num);
    dev_name = dev;
}

LdbStore::~LdbStore() {
}
bool LdbStore::InitLdbStore() {
    ldb_ptr_ = starkv_open_database(const_cast<char *>(dev_name.c_str()), (char*)"default", 1);
    if (NULL == ldb_ptr_) {
        fprintf(stderr, "open error! db_ptr_ is null!");
        return false;
    }
    printf("InitLdbStore success\n");
    return true;
}
bool LdbStore::Put(const char* key, const char* value) {
    if (NULL == ldb_ptr_) {
        fprintf(stderr, "starkv put op error! db_ptr_ is null!");
        return false;
    }
    status = starkv_put(ldb_ptr_, const_cast<char *>(key), strlen(key), const_cast<char *>(value), strlen(value), &err);
    if (status != 0) {
        fprintf(stderr, "put op failed!");
        return false;
    }
    printf("Put success\n");
    return true;
}
bool LdbStore::Get(const char* key, std::string& value) {
    if (NULL == key /*|| "" = *key*/) {
        fprintf(stderr, "key is null");
        return false;
    }
    if (NULL == ldb_ptr_) {
        fprintf(stderr, "db get op error! db_ptr_ is null!");
        return false;
    }
    size_t value_len = 0;
    char *val;
    status = starkv_get(ldb_ptr_, const_cast<char *>(key), strlen(key),  &val, &value_len, &err);
    if (status != 0) {
        fprintf(stderr, "get op failed!");
        return false;
    } else if (value_len == 0) {
        value = "";
        return false;
    }
    value = val;
    printf("Get success :%s\n", value.c_str());
    return true;
}

bool LdbStore::Delete(const char* key) {
    if (NULL == key /*|| "" = *key*/) {
        //fprintf(stderr, "key is null");
        return false;
    }
    if (NULL == ldb_ptr_) {
        fprintf(stderr, "db delete op error! db_ptr_ is null!");
        return false;
    }
    status = starkv_delete(ldb_ptr_, const_cast<char *>(key), strlen(key), &err);
    if (status != 0) {
        fprintf(stderr, "delete op failed!");
        return false;
    }
    return true;
}
}