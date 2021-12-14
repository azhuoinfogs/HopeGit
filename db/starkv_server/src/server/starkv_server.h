#ifndef __KV_SERVER_H
#define __KV_SERVER_H
#include "starkv_store_impl.h"
#include "os.h"
namespace kv_store {
class KVServer {
public:
    KVServer()
        :s("127.0.0.1", test_port), dev_name("/dev/nvme0") {
        common_db_ptr_ = new LdbStore(dev_name.c_str());    
    }
    bool InitKVStore() {
        common_db_ptr_->InitLdbStore();
        s.bind("Put", [this](std::string const& key, std::string const& value) {
            common_db_ptr_->Put(key.c_str(), value.c_str());
        });
        s.bind("Get", [this](std::string const& key) {
            std::string val;
            common_db_ptr_->Get(key.c_str(), val);
            return val;
        });
        s.bind("Delete", [this](std::string const& key) {
            return common_db_ptr_->Delete(key.c_str());
        });
        return true;
    }
    void run() {
        s.run();
    }
    ~KVServer() {}
protected:
    rpc::server s;
    LdbStore *common_db_ptr_;
    std::string dev_name;
    static RPCLIB_CONSTEXPR uint16_t test_port = rpc::constants::DEFAULT_PORT;
};

}  // end of namespace kv_store
#endif // __KV_SERVER_H
