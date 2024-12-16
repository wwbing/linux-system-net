#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<signal.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>

void communication(int cfd){
    char buf[1024];
    memset(buf, 0 ,sizeof(buf));

    while(1){
        //服务器先接受客户端的信息
        int len = recv(cfd, buf, sizeof(buf), 0);
        if(len == 0){
            printf("客户端已经断开连接\n");
            break;
        }else if(len == -1){
            printf("读取失败\n"); 
            break;
        }else{
            printf("接收到客户端发送的数据是： %s \n",buf);

            //接受到数据后回复客户端相同信息
            int tmp = send(cfd, buf, len, 0);
            if(tmp > 0){
                printf("服务器回复客户端相同的信息成功\n");
            }
        }
    }

    //因为读取失败或者客户端已经断开连接的情况退出while-->关闭通信文件描述符
    close(cfd);

}


void recycle(int no_use){
    pid_t wpid;
    while(1){
        wpid = waitpid(-1, NULL, WNOHANG);
        if(wpid == -1){
            printf("回收子进程 %d 失败\n", wpid);
            break;
        }else if(wpid > 0){
            printf("回收子进程 %d 成功\n", wpid);
            
        }else{
            //没有子进程回收
            break;
        }
    }
}

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

    //信号捕捉回收子进程
    struct sigaction act;
    act.sa_handler = recycle;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    
    sigaction(SIGCHLD, &act, NULL);     //如果子进程发生状态变化就尝试回收子进程资源


    //1 创建监听文件描述符
    int lfd = socket(AF_INET, SOCK_STREAM, 0);

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

        //有连接就创建一个子进程和客户端通信
        pid_t pid = fork();

        if(pid == 0){
            close(lfd);
            communication(cfd);
            exit(0);
        }

        close(cfd);

    }

    return 0;
}