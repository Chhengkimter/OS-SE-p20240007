#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid > 0) {
        printf("Parent (PID: %d) sleeping for 10s. Check 'ps' now for a [defunct] process.\n", getpid());
        sleep(10);
        
        printf("Parent calling wait()...\n");
        wait(NULL); 
        
        printf("Zombie reaped. Check 'ps' again—it should be gone. Sleeping 5s for observation.\n");
        sleep(5);
    } else if (pid == 0) {
        printf("Child (PID: %d) exiting immediately.\n", getpid());
        exit(0);
    }
    return 0;
}
