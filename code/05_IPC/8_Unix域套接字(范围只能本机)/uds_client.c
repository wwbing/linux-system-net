#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/unix_socket"
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_un addr;
    char buffer[BUFFER_SIZE];
    char message[BUFFER_SIZE];
    
    // 创建Unix域套接字
    if ((sock = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket创建失败");
        exit(EXIT_FAILURE);
    }
    
    // 设置服务器地址
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);
    
    // 连接到服务器
    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("connect失败");
        exit(EXIT_FAILURE);
    }
    
    printf("已连接到Unix域套接字服务器\n");
    printf("套接字路径: %s\n", SOCKET_PATH);
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
        write(sock, message, strlen(message));
        
        // 接收服务器响应
        int bytes_received = read(sock, buffer, BUFFER_SIZE);
        if (bytes_received <= 0) {
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
    
    // 关闭套接字
    close(sock);
    
    return 0;
}
