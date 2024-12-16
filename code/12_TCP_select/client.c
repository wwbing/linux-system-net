#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<stdlib.h>

                           

int main(){

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;              //ipv4类型
    addr.sin_port = htons(9999);            //端口号主机字节序(小端)通过htons转成网络字节序(大端)
    inet_pton(AF_INET, "192.168.217.131", &addr.sin_addr.s_addr);    
                                            // 通过inet_pton()把点分十进制的字符串类型的ip地址转为大端的整型ip地址


    //1.创建通信的套接字
    int cfd = socket(AF_INET, SOCK_STREAM, 0); //tcp
    if(cfd==-1){
       printf("客户端创建通信文件描述符失败\n"); 
       exit(0);
    }

    //2.连接服务器
    int ret = connect(cfd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret==-1){
       printf("客户端和服务器连接失败\n"); 
       exit(0);
    }

    //3.通信
    int num = 0;
    while(1){           //套接字在读写的时候都是阻塞的。读的时候如果空，读阻塞。写的时候没空间，写阻塞。
        
        char buf[1024];
        sprintf(buf, "hello world, %d", num++);

        //(客户端)先发送数据
        send(cfd, buf, strlen(buf)+1, 0);


        //接受服务器的返回数据
        size_t rd = recv(cfd, buf, sizeof(buf), 0);

        if(rd > 0){
            printf("===============接收到服务器回复消息 %lu 字节，内容是 %s\n", rd, buf);
        }else if(rd==0){
            printf("服务器已经断开连接\n");
            break;
        }else{
            printf("读取失败\n");
            break;
        }

        sleep(1);
    }

    //4.断开连接
    close(cfd); 

    return 0;
}