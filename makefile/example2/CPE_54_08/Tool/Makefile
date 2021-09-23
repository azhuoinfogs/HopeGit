INCLUDE_DIR := ../../include ../
DIR := -I$(INCLUDE_DIR)

all:$(OBJ_DIR)/tool.o $(OBJ_DIR)/ycodec.o

$(OBJ_DIR)/tool.o:Tools.cpp
        $(CC) -c $< $(DIR) -o $@

$(OBJ_DIR)/ycodec.o:YCodec.cpp
        $(CC) -c $< $(DIR) -o $@
