#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/epoll.h>


int main() {
    //服务器信息
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999);
    addr.sin_addr.s_addr = INADDR_ANY;
    int addrsize = sizeof(addr);

    //客户端信息
    struct sockaddr_in client;
    int clientsize = sizeof(client);
    char client_ip[INET_ADDRSTRLEN]; // 用于存储客户端IP地址的字符串

    //1 创建监听文件描述符
    int lfd = socket(AF_INET, SOCK_STREAM, 0);

    //设置端口复用,避免重复连接问题
    int optval = 1;
    setsockopt(lfd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));

    //2 绑定ip和端口
    bind(lfd, (struct sockaddr*)&addr, addrsize);
    printf("绑定成功\n");

    //3 监听
    listen(lfd, 128);
    printf("正在监听-----------------\n");

    //创建epoll树，并将监听的文件描述符添加到epoll树上
    int epfd = epoll_create(1);

    struct epoll_event ev;
    ev.data.fd = lfd;
    ev.events = EPOLLIN;
    
    epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &ev);


    while(1){
        struct epoll_event events[1024];
        int num = epoll_wait(epfd, events, 1024, -1);

        for(int i=0; i<num; i++){

            int curfd = events[i].data.fd;

            if(curfd == lfd){ //当前发发生状态变化的是监听文件描述符，表述有新连接
                int cfd = accept(lfd, NULL, NULL);

                //添加通信文件描述符到epoll树
                ev.data.fd = cfd;
                ev.events = EPOLLIN;
                epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev);
            }else{  //不是监听就一定是通信
                char buf[3];
                memset(buf, 0, sizeof(buf));
                int len = recv(curfd, buf, sizeof(buf), 0);
                if(len>0){
                    printf("接收到客户端 %d 的信息: ", curfd);
                    write(STDOUT_FILENO, buf, len); //接收到的信息写到终端，解决乱码
                    printf("\n");
                    send(curfd, buf, len, 0);
                }else if(len == 0){
                    epoll_ctl(epfd, EPOLL_CTL_DEL, curfd, NULL);
                    printf("客户端 %d 已经断开连接\n", curfd);
                    close(curfd);
                }else{
                    printf("读取失败\n");
                    exit(0);
                }


            }
        }

    }

    close(lfd);
    return 0;
}