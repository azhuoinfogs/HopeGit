#include<iostream>
#include <chrono>
#include <thread>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include "rpc/server.h"
#include "tools.h"
#include <elog_file_cfg.h>
#include <elog.h>
void elogInit(void);