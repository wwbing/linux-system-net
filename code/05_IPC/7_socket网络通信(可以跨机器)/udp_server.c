#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8889
#define BUFFER_SIZE 1024

int main() {
    int server_fd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    socklen_t client_len = sizeof(client_addr);
    
    // 创建UDP socket
    if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket创建失败");
        exit(EXIT_FAILURE);
    }
    
    // 设置服务器地址
    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    
    // 绑定socket到指定端口
    if (bind(server_fd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind失败");
        exit(EXIT_FAILURE);
    }
    
    printf("UDP服务器启动，等待消息...\n");
    
    // 接收和发送消息
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        
        // 接收消息
        int n = recvfrom(server_fd, buffer, BUFFER_SIZE, 0,
                         (struct sockaddr *)&client_addr, &client_len);
        
        if (n < 0) {
            perror("接收消息失败");
            continue;
        }
        
        printf("收到来自 %s:%d 的消息: %s", 
               inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), buffer);
        
        // 准备回复消息
        char response[BUFFER_SIZE + 32];
        sprintf(response, "服务器已收到: %s", buffer);
        
        // 发送回复
        sendto(server_fd, response, strlen(response), 0,
               (const struct sockaddr *)&client_addr, client_len);
        
        printf("已回复客户端\n");
        
        // 如果收到"exit"则退出
        if (strncmp(buffer, "exit", 4) == 0) {
            printf("收到退出命令，关闭服务器\n");
            break;
        }
    }
    
    // 关闭socket
    close(server_fd);
    
    return 0;
}
