#include "rpc/client.h"
#include <iostream>
int main() {
    rpc::client c("localhost", rpc::constants::DEFAULT_PORT);
    std::string key = "key";
    std::string value = "value";
    c.call("Put", key, value);
    std::string gvalue;
    std::string gval;
    auto current_time = c.call("Get", key).as<std::string>();
    std::cout << current_time<< std::endl;
    c.call("Delete", key);
    auto current = c.call("Get", key).as<std::string>();
    std::cout << current<< std::endl;
}
