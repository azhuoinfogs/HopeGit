INCLUDE_DIR := ../../inlcude ../Business
DIR = -I$(INCLUDE_DIR)

all:$(OBJ_DIR)/localcontrollerlist.o $(OBJ_DIR)/localcontrollerstate.o $(OBJ_DIR)/localfault.o $(OBJ_DIR)/managenetwork.o $(OBJ_DIR)/networkinformation.o $(OBJ_DIR)/optimalpathinformation.o

$(OBJ_DIR)/localcontrollerlist.o:LocalControllerList.cpp
        $(CC) -c $< $(DIR) -o $@ -std=c++0x

$(OBJ_DIR)/localcontrollerstate.o:LocalControllerState.cpp
        $(CC) -c $< $(DIR) -o $@ -std=c++0x

$(OBJ_DIR)/localfault.o:LocalFault.cpp
        $(CC) -c $< $(DIR) -o $@ -std=c++0x

$(OBJ_DIR)/managenetwork.o:ManageNetwork.cpp
        $(CC) -c $< $(DIR) -o $@ -std=c++0x

$(OBJ_DIR)/networkinformation.o:NetworkInformation.cpp
        $(CC) -c $< $(DIR) -o $@ -std=c++0x

$(OBJ_DIR)/optimalpathinformation.o:OptimalPathInformation.cpp
        $(CC) -c $< $(DIR) -o $@ -std=c++0x
