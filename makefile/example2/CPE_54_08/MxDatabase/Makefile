INCLUDE_DIR := ../MXDatabase ../../include
DIR := -I$(INCLUDE_DIR)

all:$(OBJ_DIR)/mxdatabase.o
        
$(OBJ_DIR)/mxdatabase.o:NetElementDb.cpp
        $(CC) -c $< $(DIR) -o $@ -std=c++0x
