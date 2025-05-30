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

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    
    // 创建socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket创建失败");
        exit(EXIT_FAILURE);
    }
    
    // 设置socket选项，允许地址重用
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt失败");
        exit(EXIT_FAILURE);
    }
    
    // 设置地址结构
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    // 绑定socket到指定端口
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind失败");
        exit(EXIT_FAILURE);
    }
    
    // 监听连接
    if (listen(server_fd, 3) < 0) {
        perror("listen失败");
        exit(EXIT_FAILURE);
    }
    
    printf("服务器启动，等待连接...\n");
    
    // 接受连接
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept失败");
        exit(EXIT_FAILURE);
    }
    
    printf("客户端已连接，IP: %s, 端口: %d\n", 
           inet_ntoa(address.sin_addr), ntohs(address.sin_port));
    
    // 接收并回显消息
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        
        // 接收消息
        int valread = read(new_socket, buffer, BUFFER_SIZE);
        if (valread <= 0) {
            printf("客户端断开连接\n");
            break;
        }
        
        printf("收到消息: %s", buffer);
        
        // 回显消息
        char response[BUFFER_SIZE + 32];
        sprintf(response, "服务器已收到: %s", buffer);
        send(new_socket, response, strlen(response), 0);
        printf("已回复客户端\n");
        
        // 如果收到"exit"则退出
        if (strncmp(buffer, "exit", 4) == 0) {
            printf("收到退出命令，关闭连接\n");
            break;
        }
    }
    
    // 关闭socket
    close(new_socket);
    close(server_fd);
    
    return 0;
}
