#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>


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

    // 4 初始化检测集合
    fd_set reads, tmp;

    FD_ZERO(&reads);
    FD_SET(lfd, &reads);
    int nfds = lfd + 1;

    struct timeval timeout;
    timeout.tv_sec = 5;  // 设置为5秒
    timeout.tv_usec = 0; // 设置为0微秒

    //5 不停的委托内核检测是否有满足条件的文件描述符
    while(1){
        tmp = reads; //select会修改被检测集合的值，于是创建副本去检测
        int num = select(nfds, &tmp, NULL, NULL, &timeout);
        if(num == 0){
            break;
        }else{
            printf("num = %d\n",num);
        }


        if(FD_ISSET(lfd, &tmp)){  //监听的文件描述符满足条件说明有新连接
                int cfd = accept(lfd, (struct sockaddr*)&client, &clientsize);
                // 将客户端的IP地址转换为字符串格式
                inet_ntop(AF_INET, &client.sin_addr, client_ip, INET_ADDRSTRLEN);
                // 打印客户端的IP地址和端口
                printf("客户端连接成功，IP地址：%s，端口：%d\n", client_ip, ntohs(client.sin_port));
                FD_SET(cfd, &reads);
                nfds = (cfd >= nfds) ? (cfd + 1) : nfds;
        }


        for(int i=lfd+1; i<=nfds; ++i){

            if(FD_ISSET(i, &tmp)){   //通信文件描述符满足条件说明接收到文件
                char buf[1024];
                memset(buf, 0, sizeof(buf));
                int len = recv(i, buf, sizeof(buf), 0);

                if(len > 0){
                    printf("接收到客户端 %d 的数据：%s\n",i, buf);

                    send(i, buf, strlen(buf), 0);
                    printf("已回复相同的信息\n");
                }else if(len ==0){
                    printf("客户端已经断开连接\n");
                    FD_CLR(i,&reads);
                    close(i);
                }else{
                    printf("读取出错\n");
                    break;
                }
            }
        }
    }

    close(lfd);
    return 0;
}