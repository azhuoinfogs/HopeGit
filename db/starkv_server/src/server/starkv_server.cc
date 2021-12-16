#include "starkv_server.h"
#define LOG_TAG      "starkv_server.cc"
void exit_hander(int sig)
{
  log_i("Received signal: %d", sig);
  if((sig == SIGINT) || (sig == SIGTERM)){
    log_i("exit...");
  }
  exit(0);
}

int main(int argc, char* argv[]) {
    elogInit();
    signal(SIGINT, exit_hander);
    signal(SIGTERM, exit_hander);
    kv_store::KVServer server;
    server.InitKVStore();
    server.run();
}