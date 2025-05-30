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
    int server_fd, client_fd;
    struct sockaddr_un server_addr, client_addr;
    socklen_t client_len;
    char buffer[BUFFER_SIZE];
    
    // 创建Unix域套接字
    if ((server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket创建失败");
        exit(EXIT_FAILURE);
    }
    
    // 设置服务器地址
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);
    
    // 删除可能存在的旧套接字文件
    unlink(SOCKET_PATH);
    
    // 绑定套接字
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind失败");
        exit(EXIT_FAILURE);
    }
    
    // 监听连接
    if (listen(server_fd, 5) == -1) {
        perror("listen失败");
        exit(EXIT_FAILURE);
    }
    
    printf("Unix域套接字服务器启动，等待连接...\n");
    printf("套接字路径: %s\n", SOCKET_PATH);
    
    // 接受连接
    client_len = sizeof(client_addr);
    if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len)) == -1) {
        perror("accept失败");
        exit(EXIT_FAILURE);
    }
    
    printf("客户端已连接\n");
    
    // 接收并回显消息
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        
        // 接收消息
        int bytes_received = read(client_fd, buffer, BUFFER_SIZE);
        if (bytes_received <= 0) {
            printf("客户端断开连接\n");
            break;
        }
        
        printf("收到消息: %s", buffer);
        
        // 回显消息
        char response[BUFFER_SIZE + 32];
        sprintf(response, "服务器已收到: %s", buffer);
        write(client_fd, response, strlen(response));
        
        // 如果收到"exit"则退出
        if (strncmp(buffer, "exit", 4) == 0) {
            printf("收到退出命令，关闭连接\n");
            break;
        }
    }
    
    // 关闭套接字
    close(client_fd);
    close(server_fd);
    
    // 删除套接字文件
    unlink(SOCKET_PATH);
    printf("套接字文件已删除\n");
    
    return 0;
}
