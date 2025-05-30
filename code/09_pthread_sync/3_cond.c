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


/*
总是同时使用条件变量(pthread_cond_t) 和 条件本身(data_ready) 来判断是否有数据可供消费。
主要是为了避免以下的三种情况：

    虚假唤醒    ： 条件变量可能会在没有被显式唤醒的情况下被唤醒，这种情况称为虚假唤醒。
                  如果只使用条件变量而不检查data_ready，可能会导致消费者在没有数据可消费的情况下继续处理。 

    忙等待      ： 如果不使用条件变量只使用while()和标志位判断，会导致cpu资源浪费

    多消费者    ： 只用条件变量不使用data_ready的情况下，会导致第一个消费完了，第二个接着消费已经消费过的数据
                  生产者生产后解锁，被第一个消费者抢到锁消费，消费完解锁，紧接着第二个消费者又抢到锁，接着消费已经被第一个消费者消费过的数据
*/

void* consumer(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);

        while (!data_ready) {
            pthread_cond_wait(&cond, &mutex); // 等待生产者通知，wait会先自动释放锁，然后阻塞等待被唤醒，唤醒后自动又加锁
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