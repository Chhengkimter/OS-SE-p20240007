#include <fcntl.h>    // open(), O_WRONLY, O_CREAT, O_TRUNC
#include <unistd.h>   // write(), close()
#include <string.h>   // strlen()

int main() {
int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
if (fd < 0) return 1; // Basic error check

const char *text = "Hello from Operating Systems class!\n";
write(fd, text, strlen(text));

close(fd);

const char *success = "File created successfully!\n";
write(1, success, strlen(success));

return 0;
}
