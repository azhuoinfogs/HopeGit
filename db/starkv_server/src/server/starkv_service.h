#include "os.h"
#include "starkv_store_impl.h"
#include "db_engine.pb.h"
namespace kv_store {
using namespace google::protobuf;
using std::string;
class EngineServiceImpl : public EngineService {
    public:

        EngineServiceImpl();

        ~EngineServiceImpl();

        virtual void Put(RpcController* controller,
                         const DBRequest* request,
                         DBResponse* response,
                         Closure* done);

        virtual void Get(RpcController* controller,
                         const DBRequest* request,
                         DBResponse* response,
                         Closure* done);

        virtual void Delete(RpcController* controller,
                            const DBRequest* request,
                            DBResponse* response,
                            Closure* done);
    private:
        bool EngineServiceInit();

    private:
        LdbStore *common_db_ptr_;
};
}