INCLUDE_DIR := ../../include ../
DIR := -I$(INCLUDE_DIR)

all:$(OBJ_DIR)/socket.o

$(OBJ_DIR)/socket.o:UDP.cpp
        $(CC) -c $< $(DIR) -o $@
