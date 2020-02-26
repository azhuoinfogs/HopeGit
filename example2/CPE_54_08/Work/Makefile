INCLUDE_DIR := ../../include ../
DIR := -I$(INCLUDE_DIR)

all:$(OBJ_DIR)/container.o $(OBJ_DIR)/dbsend.o $(OBJ_DIR)/dbread.o

$(OBJ_DIR)/container.o:Container.cpp
        $(CC) -c $< $(DIR) -o $@

$(OBJ_DIR)/dbread.o:DbRead.cpp
        $(CC) -c $< $(DIR) -o $@ -std=c++0x

$(OBJ_DIR)/dbsend.o:DbSend.cpp
        $(CC) -c $< $(DIR) -o $@ -std=c++0x
