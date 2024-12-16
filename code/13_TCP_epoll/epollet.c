#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <errno.h>
#include <fcntl.h>

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
    ev.events = EPOLLIN | EPOLLET;              //设置边沿模式
    
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
                //边沿模式下只通知一次，我们需要在这一次中处理完成所有的数据:循环处理，用这个较小的buf一直读
                //由于文件描述符默认是阻塞的，read/recv函数读完了之后，会阻塞，无法退出循环，于是设置当前文件描述符为非阻塞
                //设置非阻塞后，会出现异常：当缓冲区没有数据后，还是读。异常号:EAGAIN ，利用该异常作为读完的依据

                int flag = fcntl(curfd, F_GETFL);
                flag = flag | O_NONBLOCK; //追加非阻塞
                fcntl(curfd, F_SETFL, flag);

                while(1){
                    int len = recv(curfd, buf, sizeof(buf), 0);
                    if(len > 0){
                        write(STDOUT_FILENO, buf, len);
                        send(curfd, buf, len, 0);
                    }else if(len == 0){
                        printf("客户端断开连接\n");
                        close(curfd);
                        epoll_ctl(epfd, EPOLL_CTL_DEL, curfd, NULL);
                    }else{
                        if(errno = EAGAIN){
                            printf("已经读取完毕\n");
                            break;
                        }
                    }

                }
            }
        }

    }

    close(lfd);
    return 0;
}