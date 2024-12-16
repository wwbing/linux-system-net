#include<stdio.h>
#include<pthread.h>

void* p_func(void* arg){
    printf("线程创建完毕\n");
    //return (void*)74;
    pthread_exit((void*)74); //使用线程退出函数退出,参数是线程退出值
}

int main(){

    pthread_t tid;
    int *retval; //用于存储子线程退出值

    pthread_create(&tid,NULL,p_func,NULL);
    
    //主线程回收刚刚创建的子线程
    pthread_join(tid,(void**)&retval);
    printf("子线程被回收，退出值是：%d\n",(int)retval);

    //主线程退出
    pthread_exit((void*)0);
    printf("主线程退出\n");
    return 0;
}