#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
 
#define BUFFER_SIZE 512
#define log(fmt, arg...) printf("[udptest] %s:%d "fmt, __FUNCTION__, __LINE__, ##arg)
 
void main()
{
    int sock, sock2;
    int addr_len, recv_len;
    char buffer[BUFFER_SIZE] = {0};
    struct sockaddr_in addr, addr2;
 
    fd_set rfds;
    struct timeval tv;
    int retval, maxfdp1 = 0;
    
    /* 创建UDP套接字 */
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        log("create socket fail \r\n");
        return ;
    }    
    
    sock2 = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock2 == -1) {
        log("create socket2 fail \r\n");
        close(sock);
        return ;
    }  
 
    /* 设置监听地址 */
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY; 
    addr.sin_port = htons(40000);
        
    addr2.sin_family = AF_INET;
    addr2.sin_addr.s_addr = INADDR_ANY; 
    addr2.sin_port = htons(30000);
    addr_len = sizeof(struct sockaddr_in);
 
    /* 绑定本地监听地址 */
    if (0 != bind(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)))
    {
        log("bind local listening addr fail，errno : %d \r\n", errno);
        goto end;
    }  
    
    if (0 != bind(sock2, (struct sockaddr *)&addr2, sizeof(struct sockaddr_in)))
    {
        log("bind local listening addr fail，errno : %d \r\n", errno);
        goto end;
    } 
 
    /* 初始化描述符集 */
    FD_ZERO(&rfds);
 
    /* 添加到描述符集里面 */
    FD_SET(sock, &rfds);
    maxfdp1 = maxfdp1 > sock ? (maxfdp1 + 1) : (sock + 1);
 
    /* 添加到描述符集里面 */
    FD_SET(sock2, &rfds);
    maxfdp1 = maxfdp1 > sock ? (maxfdp1 + 1) : (sock + 1);
 
    /* select超时10s */
    tv.tv_sec = 10;
    tv.tv_usec = 0;
 
    /* 循环监听 */
    for (;;)
    {
        /* 只监听读事件 */
        retval = select(maxfdp1, &rfds, NULL, NULL, &tv);
        if (retval > 0)
        {
            /* 判断是否可读 */
            if (FD_ISSET(sock, &rfds))
            {
                recv_len = read(sock, buffer, sizeof(buffer));
                if (recv_len != -1)
                {
                    log("revc from sock : %s\r\n", buffer);
                    memset(buffer, 0, sizeof(buffer));
                }
            }
            if (FD_ISSET(sock2, &rfds))
            {
                recv_len = read(sock2, buffer, sizeof(buffer));
                if (recv_len != -1)
                {
                    log("revc from sock2 : %s\r\n", buffer);
                    memset(buffer, 0, sizeof(buffer));
                }
            }
        }
        else if (retval == 0)
        {
            /* select 超时 */
            log("select timeout \r\n");
        }
        else
        {
            log("select error \r\n");
        }
 
        /* 清空标志位 */
        FD_ZERO(&rfds);
 
        /* 重新设置超时 */
        tv.tv_sec = 5;
        
        /* 重新添加到select监听数组中 */
        FD_SET(sock, &rfds);
        FD_SET(sock2, &rfds);
    }
   
end:
   close(sock);
   close(sock2);
 
   return;
}