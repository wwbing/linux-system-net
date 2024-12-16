#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 10
#define NUM_CONSUMERS 3

int buffer[BUFFER_SIZE];
int in = 0;  // 写入位置
int out = 0; // 读取位置

sem_t empty; // 表示空闲缓冲区槽的信号量
sem_t full;  // 表示已占用缓冲区槽的信号量

void* producer(void* arg) {
    int item;
    while (1) {
        item = rand() % 100; // 生成一个随机项目
        sem_wait(&empty); // 等待空闲缓冲区槽

        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;
        printf("Producer: produced item %d\n", item);

        sem_post(&full); // 增加已占用缓冲区槽的计数

        sleep(1); // 模拟生产时间
    }
    return NULL;
}

void* consumer(void* arg) {
    int id = *((int*)arg);
    int item;
    while (1) {
        sem_wait(&full); // 等待已占用缓冲区槽

        item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        printf("Consumer %d: consumed item %d\n", id, item);

        sem_post(&empty); // 增加空闲缓冲区槽的计数

        sleep(1); // 模拟消费时间
    }
    return NULL;
}

int main() {
    pthread_t producer_thread, consumer_threads[NUM_CONSUMERS];
    int consumer_ids[NUM_CONSUMERS];

    // 初始化信号量
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    // 创建生产者线程
    pthread_create(&producer_thread, NULL, producer, NULL);

    // 创建消费者线程
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        consumer_ids[i] = i + 1;
        pthread_create(&consumer_threads[i], NULL, consumer, &consumer_ids[i]);
    }

    // 等待线程结束（实际上这段代码不会被执行到，因为上面的 while(1) 是无限循环）
    pthread_join(producer_thread, NULL);
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        pthread_join(consumer_threads[i], NULL);
    }

    // 销毁信号量
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}