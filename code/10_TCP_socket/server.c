#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<stdlib.h>

                           

int main(){
    //服务器信息
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;              //ipv4类型
    addr.sin_port = htons(8989);            //8989端口号主机字节序(小端)通过htons转成网络字节序(大端)
    addr.sin_addr.s_addr = INADDR_ANY;      //ip地址转大端，INADDR_ANY等价于0.0.0.0表示绑定本机的所有的ip地址
                                            //大小端的0.0.0.0都是0，存储顺序一样，无需转换
    //客户端信息
    struct sockaddr_in clientaddr;   
    int clientsize = sizeof(clientaddr);  

    //1.创建监听的套接字
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if(lfd==-1){
       printf("创建监听文件描述符失败\n"); 
       exit(1);
    }

    //2.绑定ip和端口
    int ret = bind(lfd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret==-1){
       printf("绑定ip和端口失败\n"); 
       exit(1);
    }

    //3.监听
    ret = listen(lfd, 128);  //最多监听128个连接，写死的无法更改，大于128的参数也只能128
    if(ret==-1){
       printf("服务器监听设置失败\n"); 
       exit(1);
    }

    //4.阻塞等待连接
    int cfd = accept(lfd, (struct sockaddr*)&clientaddr, &clientsize);     //返回通信文件描述符
    if(cfd == -1){
        printf("服务器和客户端建立连接失败\n"); 
        exit(0);
    }

    //5.通信
    while(1){           //套接字在读写的时候都是阻塞的。读的时候如果空，读阻塞。写的时候没空间，写阻塞。
        
        char buf[1024];
        memset(buf, 0, sizeof(buf));

        //(服务器)先接受数据
        size_t rd = recv(cfd, buf, sizeof(buf), 0);

        if(rd > 0){
            printf("recv %lu byte: %s\n", rd, buf);
        }else if(rd==0){
            printf("客户端已经断开连接\n");
            break;
        }else{
            printf("读取失败\n");
            break;
        }


        //回复数据
        send(cfd, buf, strlen(buf)+1, 0);
    }

    //6.断开连接
    close(cfd); 
    close(lfd);

    return 0;
}