#include "starkv_service.h"
#include "rpc_server.h"
#define LOG_TAG      "starkv_server.cc"
using namespace kv_store;
class Server;
void exit_hander(int sig)
{
  log_i("Received signal: %d", sig);
  if((sig == SIGINT) || (sig == SIGTERM)){
    log_i("exit...");
  }
  exit(0);
}

int main(int argc, char* argv[]) {
	  srand((unsigned int)time(NULL));
    RpcServer& rpc_server = RpcServer::GetInstance();
    EngineServiceImpl service;
    rpc_server.RegisteService(&service);
    rpc_server.Start(5, "127.0.0.1", "12321");
    rpc_server.Wait();
}