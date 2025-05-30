#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8888
#define BUFFER_SIZE 1024
#define SERVER_IP "127.0.0.1"

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    char message[BUFFER_SIZE];
    
    // 创建socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket创建失败");
        exit(EXIT_FAILURE);
    }
    
    // 设置服务器地址
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    // 将IP地址从文本转换为二进制形式
    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        perror("无效的地址/地址不支持");
        exit(EXIT_FAILURE);
    }
    
    // 连接到服务器
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("连接失败");
        exit(EXIT_FAILURE);
    }
    
    printf("已连接到服务器 %s:%d\n", SERVER_IP, PORT);
    printf("输入消息发送到服务器 (输入'exit'退出):\n");
    
    // 发送和接收消息
    while (1) {
        // 清空缓冲区
        memset(buffer, 0, BUFFER_SIZE);
        memset(message, 0, BUFFER_SIZE);
        
        // 获取用户输入
        printf("> ");
        fgets(message, BUFFER_SIZE, stdin);
        
        // 发送消息到服务器
        send(sock, message, strlen(message), 0);
        
        // 接收服务器响应
        int valread = read(sock, buffer, BUFFER_SIZE);
        if (valread <= 0) {
            printf("服务器断开连接\n");
            break;
        }
        
        printf("服务器响应: %s\n", buffer);
        
        // 如果输入"exit"则退出
        if (strncmp(message, "exit", 4) == 0) {
            printf("退出客户端\n");
            break;
        }
    }
    
    // 关闭socket
    close(sock);
    
    return 0;
}
