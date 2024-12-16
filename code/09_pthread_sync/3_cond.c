#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// 共享数据和同步变量
int shared_data = 0;
int data_ready = 0;
pthread_mutex_t mutex;
pthread_cond_t cond;

void* producer(void* arg) {
    while (1) {
        // 生成数据
        pthread_mutex_lock(&mutex);
        shared_data++;
        data_ready = 1;
        printf("Producer: produced data = %d\n", shared_data);
        pthread_cond_signal(&cond); // 通知消费者
        pthread_mutex_unlock(&mutex);
        sleep(1); // 模拟生产数据的时间
    }
    return NULL;
}

void* consumer(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        while (!data_ready) {
            pthread_cond_wait(&cond, &mutex); // 等待生产者通知
        }
        printf("Consumer: consumed data = %d\n", shared_data);
        data_ready = 0;
        pthread_mutex_unlock(&mutex);
        sleep(1); // 模拟处理数据的时间
    }
    return NULL;
}

int main() {
    pthread_t producer_thread, consumer_thread;

    // 初始化互斥锁和条件变量
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    // 创建生产者和消费者线程
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    // 等待线程结束（实际上这段代码不会被执行到，因为上面的 while(1) 是无限循环）
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    // 销毁互斥锁和条件变量
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}