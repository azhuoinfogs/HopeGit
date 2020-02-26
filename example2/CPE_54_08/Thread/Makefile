INCLUDE_DIR := ../../include ../
DIR := -I$(INCLUDE_DIR)

all:$(OBJ_DIR)/thread.o

$(OBJ_DIR)/thread.o:Thread.cpp
        $(CC) -c $< $(DIR) -o $@
