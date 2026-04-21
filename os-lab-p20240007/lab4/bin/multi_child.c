#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    printf("Parent PID: %d\n", getpid());

    for (int i = 0; i < 3; i++) {
        if (fork() == 0) {
            printf("Child %d (PID: %d) created.\n", i + 1, getpid());
            sleep(30); // Sleep long enough for you to run ps
            exit(0);
        }
    }

    // Parent waits for all three children
    for (int i = 0; i < 3; i++) {
        wait(NULL);
    }
    printf("All children finished.\n");
    return 0;
}
