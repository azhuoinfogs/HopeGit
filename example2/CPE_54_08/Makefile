INCLUDE_DIR := ../include ../
DIR := -I$(INCLUDE_DIR)

all:$(OBJ_DIR)/cpe_54_08.o $(OBJ_DIR)/stdafx.o

$(OBJ_DIR)/cpe_54_08.o:CPE_54_08.cpp
	$(CC) -c $< $(DIR) -o $@ -std=c++0x

$(OBJ_DIR)/stdafx.o:stdafx.cpp
	$(CC) -c $< $(DIR) -o $@
