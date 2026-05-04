#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t keep_running = 1;

void handle_sigint(int sig) {
    printf("\n[Signal Handler] SIGINT received! Setting keep_running = 0...\n");
    keep_running = 0;
}

void *worker_thread(void *arg) {
    int id = *(int *)arg;
    pthread_t tid = pthread_self();

    while (keep_running) {
        printf("[Thread %d] pthread_t ID: %lu | Running...\n", id, (unsigned long)tid);
        fflush(stdout);
        sleep(1);
    }

    printf("[Thread %d] Detected keep_running = 0. Exiting cleanly.\n", id);
    fflush(stdout);
    pthread_exit(NULL);
}

int main() {
    pthread_t t1, t2;
    int id1 = 1, id2 = 2;

    // Set up SIGINT handler
    struct sigaction sa;
    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    printf("[Main] Starting worker threads. Press Ctrl+C to stop.\n\n");
    fflush(stdout);

    pthread_create(&t1, NULL, worker_thread, &id1);
    pthread_create(&t2, NULL, worker_thread, &id2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("\nAll threads cleanly exited. Goodbye.\n");
    return 0;
}
