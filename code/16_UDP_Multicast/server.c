#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

// 多播的场景，服务器是发送方

int main(){
    int cfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    // 设置多播属性
    struct in_addr localInterface;
    inet_pton(AF_INET, "192.168.75.129", &localInterface.s_addr);
    setsockopt(cfd, IPPROTO_IP, IP_MULTICAST_IF, &localInterface, sizeof(localInterface));

    // 目标主机的地址信息：ip必须是多播ip,端口随意。
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999);
    inet_pton(AF_INET, "239.0.0.1", &addr.sin_addr.s_addr);
    int addrlen = sizeof(addr);

    // 发送的信息，写缓冲区 == 读缓冲区
    char buf[1024];
    int num = 0;
    
    while(1){
        // 一个服务器发送数据
        sprintf(buf, "hello world, %d", num++);
        printf("发送多播数据:%s\n", buf);
        // 多播服务器端只发送数据
        sendto(cfd, buf, strlen(buf) + 1, 0, (struct sockaddr*)&addr, addrlen);

        sleep(1);
    }

    close(cfd);

    return 0;
}