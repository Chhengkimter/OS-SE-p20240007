#include <pthread.h>
#include <unistd.h>
void* sleep_func(void* arg) { sleep(60); return NULL; }
int main() {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, sleep_func, NULL);
    pthread_create(&t2, NULL, sleep_func, NULL);
    sleep(60);
    return 0;
}
