CC=gcc
SUBDIRS=$(shell ls -l | grep ^d | awk '{if($$9 != "debug") print $$9}')
ROOT_DIR=$(shell pwd)
BIN=a.out
OBJS_DIR=debug/obj
BIN_DIR=debug/bin
CUR_SOURCE=${wildcard *.c}
CUR_OBJS=${patsubst %.c, %.o, $(CUR_SOURCE)}
CFLAG = -I$(ROOT_DIR)/common -I$(ROOT_DIR)src -Wall
export CC BIN OBJS_DIR BIN_DIR ROOT_DIR CFLAG
all:$(SUBDIRS) $(CUR_OBJS) DEBUG
$(SUBDIRS):ECHO
	make -C $@
DEBUG:ECHO
	make -C debug
ECHO:
	@echo $(SUBDIRS)
$(CUR_OBJS):%.o:%.c
	$(CC) -c $^ -o $(ROOT_DIR)/$(OBJS_DIR)/$@
clean:
	@rm $(OBJS_DIR)/*.o
	@rm -rf $(BIN_DIR)/*
