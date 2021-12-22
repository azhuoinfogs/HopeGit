#ifndef __LDB_STORE_H
#define __LDB_STORE_H

#include <string>
#include <iostream>
#include "starkv.h"
namespace kv_store {
struct Options {
};

struct ReadOptions {
};

struct WriteOptions {
};

class KVStore {
    public:
        KVStore() {
        }
        virtual ~KVStore() {
        }
        virtual bool Put(const char* key, const char* value) = 0;
        virtual bool Get(const char* key, std::string& value) = 0;
        virtual bool Delete(const char* key) = 0;
        // virtual bool StartUp() = 0;

};

class LdbStore : public KVStore {
    public:
        LdbStore(const char* db_path );
        ~LdbStore();
        bool InitLdbStore();
        virtual bool Put(const char* key, const char* value);
        virtual bool Get(const char* key, std::string& value);
        virtual bool Delete(const char* key);
    private:
        bool CreateDir(const char* path);

    private:
        char *err;
        int status;
        size_t db_num;
        std::string dev_name;
        std::string db_name;
        starkv_options_t *option_;
        starkv_t* ldb_ptr_;
};
}  // end of namespace kv_store


#endif // __LDB_STORE_H