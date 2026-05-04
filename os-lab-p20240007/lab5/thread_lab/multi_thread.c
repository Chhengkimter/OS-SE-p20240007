#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 3

void* worker_func(void* thread_id) {
    long tid = (long)thread_id;
    printf("Worker thread %ld starting...\n", tid);
    
    // Simulate some work
    long result = tid * 100;
    
    printf("Worker thread %ld finishing. Returning %ld\n", tid, result);
    pthread_exit((void*)result);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int rc;
    long t;
    void* status;

    for(t = 0; t < NUM_THREADS; t++) {
        printf("Main: creating thread %ld\n", t);
        rc = pthread_create(&threads[t], NULL, worker_func, (void*)t);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    // Wait for all threads to complete and collect their results
    for(t = 0; t < NUM_THREADS; t++) {
        rc = pthread_join(threads[t], &status);
        if (rc) {
            printf("ERROR; return code from pthread_join() is %d\n", rc);
            exit(-1);
        }
        printf("Main: joined with thread %ld, status: %ld\n", t, (long)status);
    }

    printf("Main: program completed. Exiting.\n");
    return 0;
}
