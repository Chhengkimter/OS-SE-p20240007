#include <fcntl.h>
#include <unistd.h>

int main() {
    char buffer[256];
int fd = open("output.txt", O_RDONLY);
ssize_t bytesRead;
if (fd < 0) {
const char *err = "Error: Could not open output.txt\n";
write(2, err, 33);
return 1;
}

while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
write(1, buffer, bytesRead);
}

close(fd);

return 0;
}
