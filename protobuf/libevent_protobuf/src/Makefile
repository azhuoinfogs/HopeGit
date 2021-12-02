CXX=g++
DEP_LDLIBS= \
        -levent \
        -lprotobuf \
        -lpthread
TARGET= lserver lclient
all : $(TARGET)
lserver : server.cc message.pb.cc
	$(CXX) -o $@ $^ $(DEP_LDFLAGS) $(DEP_LDLIBS)
lclient : client.cc message.pb.cc
	$(CXX) -o $@ $^ $(DEP_LDFLAGS) $(DEP_LDLIBS)
.PHONY : all clean
clean :
	rm -rf $(TARGET)