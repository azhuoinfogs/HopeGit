#定义编译器
CC = g++

OUT_PUT := cpe_54_08

#将所有的.o文件都编译到OBJ文件夹下
OBJ :=cpe_54_08.o business.o mxdatabase.o socket.o thread.o tool.o work.o
#根目录
TOP_DIR := $(PWD)

OBJ_DIR := $(TOP_DIR)/obj

BIN_DIR := $(TOP_DIR)/CPE_54_08

SRC_BUSINESS_DIR := $(BIN_DIR )/Business
SRC_MXDATABASE_DIR := $(BIN_DIR )/MxDatabase
SRC_SOCKET_DIR := $(BIN_DIR )/Socket
SRC_THREAD_DIR := $(BIN_DIR )/Thread
SRC_TOOL_DIR := $(BIN_DIR )/Tool
SRC_WORK_DIR := $(BIN_DIR )/Work

SUB_DIR := CPE_54_08\
           CPE_54_08/Business\
           CPE_54_08/MXDatabase\
           CPE_54_08/Socket\
           CPE_54_08/Thread\
           CPE_54_08/Tool\
           CPE_54_08/Work

LIB_PATH := $(TOP_DIR)/lib
LINK_LIB := -L$(LIB_PATH) -lbsapi -L$(LIB_PATH) -lMxHandlePool -L$(LIB_PATH) -lServerData -L$(LIB_PATH) -lmxcomlib -L$(LIB_PATH) -lmxdbvar -L$(LIB_PATH) -lchen -luuid

export CC OBJ TOP_DIR OBJ_DIR BIN_DIR SRC_BUSINESS_DIR SRC_MXDATABASE_DIR\
        SRC_SOCKET_DIR SRC_THREAD_DIR SRC_TOOL_DIR SRC_WORK_DIR

all:$(SUB_DIR) $(OUT_PUT)

$(SUB_DIR):ECHO
	make -C $@ 

ECHO:
	@echo $(SUB_DIR)
	@echo begin compile
        
$(OUT_PUT):$(OBJ_DIR)/cpe_54_08.o $(OBJ_DIR)/dbsend.o $(OBJ_DIR)/localcontrollerstate.o $(OBJ_DIR)/mxdatabase.o $(OBJ_DIR)/optimalpathinformation.o $(OBJ_DIR)/stdafx.o $(OBJ_DIR)/tool.o\
                $(OBJ_DIR)/localfault.o $(OBJ_DIR)/localcontrollerlist.o $(OBJ_DIR)/dbread.o $(OBJ_DIR)/container.o $(OBJ_DIR)/networkinformation.o $(OBJ_DIR)/managenetwork.o $(OBJ_DIR)/ycodec.o\
                 $(OBJ_DIR)/thread.o $(OBJ_DIR)/socket.o
	g++ -o $(OUT_PUT)  $(OBJ_DIR)/cpe_54_08.o $(OBJ_DIR)/dbsend.o $(OBJ_DIR)/localcontrollerstate.o $(OBJ_DIR)/mxdatabase.o $(OBJ_DIR)/optimalpathinformation.o $(OBJ_DIR)/stdafx.o $(OBJ_DIR)/tool.o\
                $(OBJ_DIR)/localfault.o $(OBJ_DIR)/localcontrollerlist.o $(OBJ_DIR)/dbread.o $(OBJ_DIR)/container.o $(OBJ_DIR)/networkinformation.o $(OBJ_DIR)/managenetwork.o $(OBJ_DIR)/ycodec.o $(OBJ_DIR)/thread.o\
                $(OBJ_DIR)/socket.o $(LINK_LIB)  -lpthread
clean:
	rm -rf $(OBJ_DIR)/*.o $(OUT_PUT)
