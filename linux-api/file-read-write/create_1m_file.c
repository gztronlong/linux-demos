
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#define INPUT_FILE_NAME     "1m_data.file"

int main() {
    int i;
    char data[1024];

    // If not exist, creates it, if do, truncates it.
    int fd = open(INPUT_FILE_NAME, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        printf("Open file %s failed, exiting...\n", INPUT_FILE_NAME);
        exit(1);
    }

    for (i = 0; i < 1024; i++) {
        write(fd, data, 1024);
    }

    close(fd);
    return 0;
}
