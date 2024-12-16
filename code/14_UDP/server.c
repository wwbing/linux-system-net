#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>


int main(){

    int cfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    //绑定本机指定ip和端口用于udp通信接收udp信息
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999);
    addr.sin_addr.s_addr = INADDR_ANY;
    bind(cfd, (struct sockaddr*)&addr, sizeof(addr));    // 绑定端口和IP


    //保存客户端的信息，才能发送给客户端
    char ip[24];
    struct sockaddr_in client_addr;
    int client_addr_len = sizeof(client_addr);


    char buf[1024];
    while(1){
        //服务器先接受数据
        recvfrom(cfd, buf, 1024, 0, (struct sockaddr*)&client_addr, &client_addr_len);
        printf("recved from ip:%s, port: %d, buf: %s\n", 
                inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, ip, sizeof(ip)),
                ntohs(client_addr.sin_port),
                buf);

        sendto(cfd, buf, strlen(buf)+1, 0, (struct sockaddr*)&client_addr, client_addr_len);
    
    }

    close(cfd);

    return 0;
}