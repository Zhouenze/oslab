#include <pthread.h>
#include <stdio.h>

enum State {
    IDLE,
    WAITING,
    ACTIVE
};

#define MAX_THREADS 10
enum State flags[MAX_THREADS];
int turn = 0;

void* process(void* args) {
    int j;
    int tid = *(int*)args;
    printf("Thread %d start\n", tid);

    //循环多次以进行观察
    for (int i = 0; i < 4; ++i) {

        //等待
        do {
            flags[tid] = WAITING;
            j = turn;
            while (j != tid) {
                if (flags[j] != IDLE)
                    j = turn;
                else
                    j = (j + 1) % MAX_THREADS;
            }
            flags[tid] = ACTIVE;
            j = 0;
            while ((j < MAX_THREADS) && (j == tid || flags[j] != ACTIVE))
                ++j;
        } while (j != MAX_THREADS);
        turn = tid;

        //工作
        printf("Thread %d working\n", tid);

        //释放
        j = (turn + 1) % MAX_THREADS;
        while (flags[j] == IDLE)
            j = (j + 1) % MAX_THREADS;
        turn = j;
        flags[tid] = IDLE;
    }

    return NULL;
}

int main() {
    int thread_id[MAX_THREADS];
    pthread_t thread[MAX_THREADS];
    for (int i = 0; i < MAX_THREADS; i++) {
        flags[i] = IDLE;
        thread_id[i] = i;
    }

    int arra[MAX_THREADS] = {5,9,7,2,8,0,6,4,3,1};
    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_create(&thread[arra[i]], NULL, process, (void*)&thread_id[arra[i]]);
    }

    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(thread[i], NULL);
    }

    return 0;
}