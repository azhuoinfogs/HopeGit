OBJS=stardb.o
CC=gcc
XX=g++
CFLAGS=-Wall -O -g
LDFLAGS= -L ./ -ltaos -lpthread -lm 

TARGET=stardb

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o:%.cpp
	$(XX) $(CFLAGS) -c $< -o $@

SOURCES=$(wildcard *.c *.cpp)
OBJS=$(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))

$(TARGET):$(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(TARGET)
	chmod a+x $(TARGET)
clean:
	rm -rf *.o stardb

