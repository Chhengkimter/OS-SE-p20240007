#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int global_var = 10;

void* thread_func(void* arg) {
    global_var += 20;
    printf("Thread: global_var = %d\n", global_var);
    return NULL;
}

int main() {
    pthread_t thread;
    pthread_create(&thread, NULL, thread_func, NULL);
    pthread_join(thread, NULL); // Wait for thread to finish

    printf("Main thread: global_var = %d\n", global_var);
    return 0;
}
