#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

// 定义消息结构体，需要与发送端一致
struct msg_st {
    long int msg_type;
    char text[512];
};

int main() {
    int msgid;
    struct msg_st data;
    long int msg_to_receive = 1; // 接收类型为1的消息
    int running = 1;
    
    // 创建消息队列
    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
    if (msgid == -1) {
        fprintf(stderr, "获取消息队列失败\n");
        exit(EXIT_FAILURE);
    }
    
    printf("等待接收消息...\n");
    
    // 循环接收消息
    while (running) {
        // 接收消息
        if (msgrcv(msgid, (void*)&data, sizeof(struct msg_st) - sizeof(long int), msg_to_receive, 0) == -1) {
            fprintf(stderr, "接收消息失败\n");
            exit(EXIT_FAILURE);
        }
        
        printf("收到消息: %s", data.text);
        
        // 收到end消息时结束
        if (strncmp(data.text, "end", 3) == 0) {
            running = 0;
        }
    }
    
    // 删除消息队列
    if (msgctl(msgid, IPC_RMID, 0) == -1) {
        fprintf(stderr, "删除消息队列失败\n");
        exit(EXIT_FAILURE);
    }
    
    printf("消息队列已删除\n");
    
    return 0;
}
