CC = gcc
C++ = g++
INSTALL_PATH ?= /usr
DEBUG_FLAGS = -DDEBUG_FLAG -g
LDFLAGS = -ldl -lpthread
INCLUDE = -I ./include
OBJS =  ./src/array.c\
		./src/compare.c\
		./src/status.c\
		./src/skiplist.c\
		./src/util.c\
		./src/mem.c\
		./src/debug.c\
		./src/bitmap.c\
		./src/data_block.c\
		./src/kv_store.c\
		./src/starkv.c
SKIPLIST_TEST_OBJS =  ./test/skiplist_test.c
STARKV_TEST_OBJS =  ./test/starkv_test.c
MEM_TEST_OBJS =  ./test/mem_test.c
BIT_TEST_OBJS =  ./test/bitmap_test.c
BLOCK_TEST_OBJS =  ./test/datablock_test.c
UTIL_TEST_OBJS =  ./test/util_test.c
UNIT_TEST_OBJS =  ./test/unit_test.c
LIBRARY = ./libstarkv.so
.PHONY:bench
release:
	$(CC) $(INCLUDE) $(OBJS) -fPIC -shared $(LDFLAGS) -o $(LIBRARY)
debug:
	$(CC) $(INCLUDE) $(OBJS) -fPIC -shared $(LDFLAGS) $(DEBUG_FLAGS) -o $(LIBRARY)
install:$(LIBRARY)
	install -C -m 755 $(LIBRARY) $(INSTALL_PATH)/lib
	install -C -m 644 include/starkv.h $(INSTALL_PATH)/include
uninstall:
	rm $(INSTALL_PATH)/lib/libstarkv.so
	rm $(INSTALL_PATH)/include/starkv.h
starkv_test:
	$(CC)  $(STARKV_TEST_OBJS)  $(INCLUDE) -L ./ -lstarkv -ldl -g -o ./test/starkv_test
skiplist_test:
	$(CC)  $(SKIPLIST_TEST_OBJS)  $(INCLUDE) -L ./ -lstarkv -g -o ./test/skip_test
mem_test:
	$(CC)  $(MEM_TEST_OBJS)  $(INCLUDE) -L ./ -lstarkv -g -o ./test/mem_test
bit_test:
	$(CC)  $(BIT_TEST_OBJS)  $(INCLUDE) -L ./ -lstarkv -g -o ./test/bit_test
block_test:
	$(CC)  $(BLOCK_TEST_OBJS)  $(INCLUDE) -L ./ -lstarkv -g -o ./test/block_test
util_test:
	$(CC)  $(UTIL_TEST_OBJS)  $(INCLUDE) -L ./ -lstarkv -g -o ./test/util_test
unit_test:
	$(CC)  $(UNIT_TEST_OBJS)  $(INCLUDE) -L ./ -lstarkv -g -o ./test/unit_test
bench:
	$(CC) bench/bench.c  -lstarkv -o ./bench/kv_bench
.PHONY:clean
clean:
	-rm -f $(LIBRARY)
	-rm -f ./test/starkv_test
	-rm -f ./test/skip_test
	-rm -f ./test/bit_test
	-rm -f ./test/mem_test
	-rm -f ./test/util_test
	-rm -f ./bench/kv_bench