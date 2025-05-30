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
    int running = 1;
    
    // 生成与写入进程相同的key
    key = ftok("/tmp", 'a');
    if (key == -1) {
        perror("ftok失败");
        exit(EXIT_FAILURE);
    }
    
    // 获取已存在的共享内存
    shmid = shmget(key, SHM_SIZE, 0666);
    if (shmid == -1) {
        perror("shmget失败");
        exit(EXIT_FAILURE);
    }
    
    printf("共享内存访问成功，ID: %d\n", shmid);
    
    // 连接到共享内存
    shm_addr = shmat(shmid, NULL, 0);
    if (shm_addr == (char *)-1) {
        perror("shmat失败");
        exit(EXIT_FAILURE);
    }
    
    printf("共享内存已连接到地址: %p\n", shm_addr);
    printf("开始从共享内存读取数据...\n");
    
    // 循环读取共享内存中的数据
    while (running) {
        printf("共享内存内容: %s", shm_addr);
        
        // 如果检测到exit则退出
        if (strncmp(shm_addr, "exit", 4) == 0) {
            running = 0;
        }
        
        sleep(1);
    }
    
    // 分离共享内存
    if (shmdt(shm_addr) == -1) {
        perror("shmdt失败");
        exit(EXIT_FAILURE);
    }
    
    // 删除共享内存
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl(IPC_RMID)失败");
        exit(EXIT_FAILURE);
    }
    
    printf("共享内存已删除\n");
    
    return 0;
}
