#include <stdio.h>
#include <stdatomic.h>
#include <pthread.h>

atomic_bool flag[2];
atomic_int turn;
int shared = 0;

void *process0(void *arg) {
    for (int i = 0; i < 5; i++) {
        atomic_store(&flag[0], 1);
        atomic_store(&turn, 1);

        while (atomic_load(&flag[1]) && atomic_load(&turn) == 1)
            ;

        shared++;
        printf("Process 0: shared = %d\n", shared);

        atomic_store(&flag[0], 0);
    }
    return NULL;
}

void *process1(void *arg) {
    for (int i = 0; i < 5; i++) {
        atomic_store(&flag[1], 1);
        atomic_store(&turn, 0);

        while (atomic_load(&flag[0]) && atomic_load(&turn) == 0)
            ;

        shared++;
        printf("Process 1: shared = %d\n", shared);

        atomic_store(&flag[1], 0);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;

    atomic_init(&flag[0], 0);
    atomic_init(&flag[1], 0);
    atomic_init(&turn, 0);

    pthread_create(&t1, NULL, process0, NULL);
    pthread_create(&t2, NULL, process1, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Final shared value = %d\n", shared);
    return 0;
}