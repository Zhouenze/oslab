#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

int M = 4, N = 5;                   //题目设置的界
int numA, numB;                     //AB库中目前有的AB数量
sem_t Acont, Bcont, Ccont;          //ABCcont表示“尝试运行”，例如：当B或者C完成了自己的操作时A可能可以运行，因此唤醒之让他尝试运行，随后A检查是否实际满足运行条件，可以运行则运行，否则继续沉睡
sem_t mutex;                        //mutex保证检查和更改numAB的原子性

void* proA (void* arg) {
    for (int i = 0; i < 100; ++i) {
        sem_wait(&Acont);                               //等到时表示现在可能可以运行
        sem_wait(&mutex);                               //为保证检查和更改numAB的原子性申请mutex
        if (numA >= M || numA - numB >= N) {            //不满足条件则释放mutex并continue，即继续等待可能可以运行的时机
            sem_post(&mutex);
            continue;
        }
        ++numA;                                         //满足条件则放入A并输出
        printf("put  A now A %d B %d\n", numA, numB);
        sem_post(&mutex);                               //释放mutex
        sem_post(&Ccont);                               //放入A后BC可能可以运行，因此唤醒他们进行尝试
        sem_post(&Bcont);
    }
    return NULL;
}

void* proB (void* arg) {                                //注释同上
    for (int i = 0; i < 100; ++i) {
        sem_wait(&Bcont);
        sem_wait(&mutex);
        if (numB >= N || numB - numA >= M) {
            sem_post(&mutex);
            continue;
        }
        ++numB;
        printf("put  B now A %d B %d\n", numA, numB);
        sem_post(&mutex);
        sem_post(&Ccont);
        sem_post(&Acont);
    }
    return NULL;
}

void* proC (void* arg) {                                //注释类似AB
    for (int i = 0; i < 100; ++i) {
        sem_wait(&Ccont);
        sem_wait(&mutex);
        if (numA <= 0 || numB <= 0) {
            sem_post(&mutex);
            continue;
        }
        --numA;
        --numB;
        printf("make C now A %d B %d\n", numA, numB);
        sem_post(&mutex);
        sem_post(&Acont);
        sem_post(&Bcont);
    }
    return NULL;
}

int main() {

    numA = numB = 0;                //初始时两个库都空
    sem_init(&Acont, 0, 1);         //ABC都要尝试运行
    sem_init(&Bcont, 0, 1);
    sem_init(&Ccont, 0, 1);
    sem_init(&mutex, 0, 1);         //mutex是初始为1的信号灯保证原子操作

    pthread_t threads[3];
    pthread_create(&threads[0], 0, proA, NULL);
    pthread_create(&threads[1], 0, proB, NULL);
    pthread_create(&threads[2], 0, proC, NULL);
    for (int i = 0; i < 3; ++i) {
        pthread_join(threads[i], 0);
    }

    return 0;
}