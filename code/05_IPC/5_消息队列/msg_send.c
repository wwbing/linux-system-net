#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

// 定义消息结构体
struct msg_st {
    long int msg_type;
    char text[512];
};

int main() {
    int msgid;
    struct msg_st data;
    char buffer[512];
    int running = 1;
    
    // 创建消息队列
    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
    if (msgid == -1) {
        fprintf(stderr, "创建消息队列失败\n");
        exit(EXIT_FAILURE);
    }
    
    // 循环发送消息
    while (running) {
        printf("输入要发送的消息: ");
        fgets(buffer, 512, stdin);
        
        data.msg_type = 1; // 消息类型设为1
        strcpy(data.text, buffer);
        
        // 发送消息
        if (msgsnd(msgid, (void*)&data, sizeof(struct msg_st) - sizeof(long int), 0) == -1) {
            fprintf(stderr, "发送消息失败\n");
            exit(EXIT_FAILURE);
        }
        
        // 输入end结束
        if (strncmp(buffer, "end", 3) == 0) {
            running = 0;
        }
        
        sleep(1);
    }
    
    printf("消息发送完成\n");
    
    return 0;
}
