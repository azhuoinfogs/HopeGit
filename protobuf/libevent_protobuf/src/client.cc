#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/util.h>

#include "message.pb.h"

using namespace std;

void cmd_msg_cb(int fd, short events, void *arg);

void server_msg_cb(bufferevent *bev, void *arg);

void event_cb(bufferevent *bev, short event, void *arg);

static int gid = 1;

int main(int argc, char **argv) {
        if (argc < 3) {
                printf("please input IP and port\n");
                return 1;
        }
        event_base *base = event_base_new();
        bufferevent *bev = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE);
        event *ev_cmd = event_new(base, STDIN_FILENO,
                        EV_READ|EV_PERSIST,
                        cmd_msg_cb, (void *)bev);
        event_add(ev_cmd, NULL);
        sockaddr_in server_addr;
        memset(&server_addr, 0, sizeof(server_addr));

        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(atoi(argv[2]));
        inet_aton(argv[1], &server_addr.sin_addr);

        bufferevent_socket_connect(bev, (sockaddr*)&server_addr, sizeof(server_addr));

        bufferevent_setcb(bev, server_msg_cb, NULL, event_cb, (void*)ev_cmd);
        bufferevent_enable(bev, EV_READ|EV_PERSIST);

        event_base_dispatch(base);

        printf("Finish\n");
        return 0;

}

void cmd_msg_cb(int fd, short events, void *arg) {
        char msg[1024];
        int ret = read(fd, msg, sizeof(msg));
        if (ret < 0) {
                perror("read error.\n");
                exit(1);
        }

        // protobuf
        PMessage pmsg;
        pmsg.set_id(gid++);
        pmsg.set_num(rand());
        pmsg.set_str(msg);

        string sendbuf;
        pmsg.SerializeToString(&sendbuf);

        // processing network transfer
        bufferevent *bev = (bufferevent *)arg;
        bufferevent_write(bev, sendbuf.c_str(), sendbuf.length());
}

void server_msg_cb(bufferevent *bev, void *arg) {
        char msg[1024];

        size_t len = bufferevent_read(bev, msg, sizeof(msg)-1);
        msg[len] = '\0';

        PMessage pmsg;
        pmsg.ParseFromArray((const void*)msg, len);

        printf("Recv %d, %d, %s from server.\n", pmsg.id(), pmsg.num(), pmsg.str().c_str());
}

void event_cb(bufferevent *bev, short eventid, void *arg) {
        if (eventid & BEV_EVENT_EOF) {
                printf("Connection closed.\n");
        }
        else if (eventid & BEV_EVENT_ERROR) {
                printf("Some other error.\n");
        }
        else if (eventid & BEV_EVENT_CONNECTED) {
                printf("Client has successfully connected.\n");
                return;
        }

        bufferevent_free(bev);
        event *ev = (event *)arg;
        event_free(ev);
}