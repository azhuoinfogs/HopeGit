#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <event2/event.h>
#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/thread.h>
#include "message.pb.h"
using namespace std;

void listener_cb(evconnlistener *listener, evutil_socket_t fd,
                sockaddr *sock, int socklen, void *arg);

void socket_read_cb(bufferevent *bev, void *arg);

void socket_event_cb(bufferevent *bev, short events, void *arg);

int main(int argc, char **argv) {
        sockaddr_in sin;
        memset(&sin, 0, sizeof(sockaddr_in));
        sin.sin_family = AF_INET;
        sin.sin_port = htons(8899);
        event_base *base = event_base_new();
        evconnlistener *listener
                = evconnlistener_new_bind(base,
                        listener_cb, base,
                        LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE,
                        10, (sockaddr*)&sin, sizeof(sockaddr_in));

        event_base_dispatch(base);
        evconnlistener_free(listener);
        event_base_free(base);

}

void listener_cb(evconnlistener *listener, evutil_socket_t fd,
        sockaddr *sock, int socklen, void *arg) {
        printf("accept a client %d\n", fd);
        event_base *base = (event_base *)arg;
        bufferevent *bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
        bufferevent_setcb(bev, socket_read_cb, NULL, socket_event_cb, NULL);
        bufferevent_enable(bev, EV_READ|EV_PERSIST);
}

void socket_read_cb(bufferevent *bev, void *arg) {
        char msg[4096];
        size_t len = bufferevent_read(bev, msg, sizeof(msg)-1);
        msg[len] = '\0';
        PMessage pmsg;
        pmsg.ParseFromArray((const void*)msg, len);

        printf("Server read the data:%i, %i, %s\n", pmsg.id(), pmsg.num(), pmsg.str().c_str());

        pmsg.set_str("I have read your data.");
        string sendbuf;
        pmsg.SerializeToString(&sendbuf);
        bufferevent_write(bev, sendbuf.c_str(), sendbuf.length());
}

void socket_event_cb(bufferevent *bev, short events, void *arg) {
        if (events & BEV_EVENT_EOF) {
                printf("connection close\n");
        }
        else if (events & BEV_EVENT_ERROR) {
                printf("some other error\n");
        }
        bufferevent_free(bev);
}