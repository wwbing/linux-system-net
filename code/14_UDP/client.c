#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>


int main(){
    int cfd = socket(AF_INET, SOCK_DGRAM, 0);

    //目标主机的地址信息：ip端口
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999);
    inet_pton(AF_INET, "192.168.75.129", &addr.sin_addr.s_addr);
    int addrlen = sizeof(addr);


    //发送的信息，写缓冲区 == 读缓冲区
    char buf[1024];
    int num = 0;
    

    char ip[24];
    struct sockaddr_in seraddr;
    int seraddr_len = sizeof(seraddr);

    while(1){
        sprintf(buf, "hello world, %d", num++);
        sendto(cfd, buf, strlen(buf)+1, 0, (struct sockaddr*)&addr, addrlen);

        recvfrom(cfd, buf, strlen(buf)+1, 0, (struct sockaddr*)&seraddr, &seraddr_len);

        printf("recv from server ip: %s, port : %d, buf: %s\n",
                inet_ntop(AF_INET, &seraddr.sin_addr.s_addr, ip, sizeof(ip)),
                ntohs(seraddr.sin_port),
                buf
                );

        sleep(1);
    }

    close(cfd);

    return 0;
}