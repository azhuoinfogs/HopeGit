
all:
	gcc -c channel.cpp server.cpp rpc_meta.pb.cc -lpthread
	ar rcs libsrpc.a channel.o server.o rpc_meta.pb.o
