#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Real UID:      %d\n", getuid());
    printf("Effective UID: %d\n", geteuid());
    printf("Running as:    ");
    fflush(stdout);
    execlp("whoami", "whoami", NULL);
    return 0;
}
