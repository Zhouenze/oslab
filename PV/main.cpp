#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

int M = 4, N = 5;
sem_t Acont, Bcont, Ccont, mutex;
int numA, numB;

void* proA (void* arg) {
    for (int i = 0; i < 100; ++i) {
        sem_wait(&Acont);
        sem_wait(&mutex);
        if (numA >= M || numA - numB >= N) {
            sem_post(&mutex);
            continue;
        }
        ++numA;
        printf("put  A now A %d B %d\n", numA, numB);
        sem_post(&mutex);
        sem_post(&Ccont);
        sem_post(&Bcont);
    }
    return NULL;
}

void* proB (void* arg) {
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

void* proC (void* arg) {
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
    numA = numB = 0;
    sem_init(&Acont, 0, 1);
    sem_init(&Bcont, 0, 1);
    sem_init(&Ccont, 0, 1);
    sem_init(&mutex, 0, 1);

    pthread_t threads[3];
    pthread_create(&threads[0], 0, proA, NULL);
    pthread_create(&threads[1], 0, proB, NULL);
    pthread_create(&threads[2], 0, proC, NULL);

    for (int i = 0; i < 3; ++i) {
        pthread_join(threads[i], 0);
    }
    return 0;
}