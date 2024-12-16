#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>


//广播场景中客户端是接受方

int main(){

    int cfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    // 本机测试，设置端口复用
    int reuse = 1;
    setsockopt(cfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));


    //绑定本机指定ip和端口用于udp广播通信
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999);
    addr.sin_addr.s_addr = INADDR_ANY;  //接收端也是零地址，因为广播的信息回来到本机，我也是绑定本机的ip和端口接收
    bind(cfd, (struct sockaddr*)&addr, sizeof(addr));    



    //保存一下发送方的信息
    char ip[24];
    struct sockaddr_in ser_addr;
    int ser_addr_len = sizeof(ser_addr);

    
    char buf[1024];
    while(1){
        //多个客户端接受数据
        recvfrom(cfd, buf, 1024, 0, (struct sockaddr*)&ser_addr, &ser_addr_len);

        printf("recved from 广播： ip:%s, port: %d, buf: %s\n", 
                inet_ntop(AF_INET, &ser_addr.sin_addr.s_addr, ip, sizeof(ip)),
                ntohs(ser_addr.sin_port),
                buf); 
    }

    close(cfd);

    return 0;
}