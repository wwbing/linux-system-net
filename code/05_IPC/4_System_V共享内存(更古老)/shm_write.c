#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 1024  // 共享内存大小

int main() {
    key_t key;
    int shmid;
    char *shm_addr;
    char buffer[SHM_SIZE];
    int running = 1;
    
    // 生成唯一key
    key = ftok("/tmp", 'a');
    if (key == -1) {
        perror("ftok失败");
        exit(EXIT_FAILURE);
    }
    
    // 创建共享内存
    shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget失败");
        exit(EXIT_FAILURE);
    }
    
    printf("共享内存创建成功，ID: %d\n", shmid);
    
    // 连接到共享内存
    shm_addr = shmat(shmid, NULL, 0);
    if (shm_addr == (char *)-1) {
        perror("shmat失败");
        exit(EXIT_FAILURE);
    }
    
    printf("共享内存已连接到地址: %p\n", shm_addr);
    
    // 循环写入数据到共享内存
    while (running) {
        printf("输入要写入共享内存的内容 (输入'exit'退出): ");
        fgets(buffer, SHM_SIZE, stdin);
        
        // 写入数据到共享内存
        strcpy(shm_addr, buffer);
        printf("数据已写入共享内存: %s\n", buffer);
        
        // 输入exit退出
        if (strncmp(buffer, "exit", 4) == 0) {
            running = 0;
        }
        
        sleep(1);
    }
    
    // 分离共享内存
    if (shmdt(shm_addr) == -1) {
        perror("shmdt失败");
        exit(EXIT_FAILURE);
    }
    
    printf("已分离共享内存\n");
    
    return 0;
}
