#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>

pthread_mutex_t m1,m2;
char val1[100];
char val2[100];

void* pfunc1(void* arg){
    char* p = (char*)arg;
    while(1){
        pthread_mutex_lock(&m1);
        sleep(1);
        pthread_mutex_lock(&m2);    //环状死锁

        strcpy(val1, p);
        printf("%s\n",val1);
        
        pthread_mutex_unlock(&m1);
        pthread_mutex_unlock(&m2);

        sleep(1);
    }
}

void* pfunc2(void* arg){
    char* p = (char*)arg;
    while(1){
        pthread_mutex_lock(&m2);
        sleep(1);
        pthread_mutex_lock(&m1);    //环状死锁

        strcpy(val2, p);
        printf("%s\n",val2);

        pthread_mutex_unlock(&m2);
        pthread_mutex_unlock(&m1);
        
        sleep(1);
    }
}

int main(){

    pthread_t tid1,tid2;
    pthread_mutex_init(&m1,NULL);
    pthread_mutex_init(&m2,NULL);

    pthread_create(&tid1,NULL,pfunc1,(void*)"func1");
    pthread_create(&tid2,NULL,pfunc2,(void*)"func2");


    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    return 0;
}