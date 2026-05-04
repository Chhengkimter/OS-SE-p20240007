#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void sig_handler(int signo) {
    if (signo == SIGINT) {
        printf("\n[Signal Caught] Received SIGINT (Ctrl+C). Performing graceful shutdown...\n");
        // Simulate cleanup
        sleep(1);
        printf("Cleanup complete. Exiting safely.\n");
        exit(0);
    } if (signal(SIGKILL, sig_handler) == SIG_ERR) {
        printf("Warning: Cannot catch SIGKILL\n");
    } else if (signo == SIGTERM) {
        printf("\n[Signal Caught] Received SIGTERM (kill). Saving state...\n");
        exit(0);
    }
}

int main() {
    // Register signal handlers
    if (signal(SIGINT, sig_handler) == SIG_ERR) {
        printf("Cannot catch SIGINT\n");
    }
    if (signal(SIGTERM, sig_handler) == SIG_ERR) {
        printf("Cannot catch SIGTERM\n");
    }

    printf("Process running with PID: %d\n", getpid());
    printf("Try pressing Ctrl+C or sending 'kill %d' from another terminal.\n", getpid());
    printf("Entering infinite loop...\n");

    while(1) {
        printf("Working...\n");
        sleep(2);
    }
    return 0;
}
