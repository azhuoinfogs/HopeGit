#include<iostream>
#include <chrono>
#include <thread>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <elog_file_cfg.h>
#include <elog.h>
#include "rpc/server.h"
#include <tools.h>

void elogInit(void);
bool Split(std::string& splited_str, 
           const std::string& split_ch, 
           std::vector<std::string>& split_vec);

bool Split(const char* splited_chs, 
           const std::string& split_ch, 
           std::vector<std::string>& split_vec);