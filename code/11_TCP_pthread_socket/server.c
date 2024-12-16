#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<signal.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<pthread.h>


void* communication(void* cfd);

int main(){

    //服务器信息
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999);
    addr.sin_addr.s_addr = INADDR_ANY;
    int addrsize = sizeof(addr);

    //客户端信息
    struct sockaddr_in client;
    int clientsize = sizeof(client);


    //1 创建监听文件描述符
    int lfd = socket(AF_INET, SOCK_STREAM, 0);

    //设置端口复用,避免重复连接问题
    int optval = 1;
    setsockopt(lfd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));

    //绑定ip和端口
    bind(lfd, (struct sockaddr*)&addr, addrsize);
    printf("绑定成功\n");

    //2 监听
    listen(lfd, 128);
    printf("正在监听-----------------\n");

    //因为要接受多客户端的连接因此要循环accept
    while(1){
        //3 阻塞等待客户端请求并且建立连接
        int cfd = accept(lfd, (struct sockaddr*)&client, &clientsize);
        if(cfd == -1){
            printf("接收到客户端请求但是建立连接失败，尝试重新连接,continue accept-----\n");
            continue;
        }

        char clientip[24];
        printf("客户端的IP地址：%s, 端口：%d\n", 
        inet_ntop(AF_INET, &client.sin_addr.s_addr, clientip, sizeof(clientip)),
        ntohs(client.sin_port));

        pthread_t pid;
        //有连接就创建一个线程和客户端通信
        pthread_create(&pid, NULL, communication, (void*)&cfd);
        pthread_detach(pid);    //设置分离，结束后自动回收
    
    }


    return 0;
}


void* communication(void* cfd){
    int cfd_new = *(int*)cfd;  //cfd是主线程的文件描述符，会更新被所有线程共享，所以不能直接用，
                                //根据线程间栈空间不共享的特点，用cfd_new保存当前连接的文件描述符来操作

    char buf[1024];
    memset(buf,0,sizeof(buf));

    while(1){
        //接收信息
        int len = recv(cfd_new, buf, sizeof(buf), 0);
        if(len == 0){
            //客户端已经断开连接
            pthread_exit(0);
        }else if(len == -1){
            printf("服务器读取信息失败\n");
        }else{
            printf("当前线程的通信文件描述符: %d \n",cfd_new);
            printf("接收到客户端信息，并回复相同的信息： %s\n",buf);
            send(cfd_new ,buf,sizeof(buf), 0);
        }
    }
}