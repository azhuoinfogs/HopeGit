#include "starkv_server.h"
#include "tools.h"
int main(int argc, char* argv[]) {
    kv_store::KVServer server;
    server.InitKVStore();
    server.run();
}