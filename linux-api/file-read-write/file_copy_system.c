
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#define INPUT_FILE_NAME     "1m_data.file"
#define OUTPUT_FILE_NAME    "1m_data.file.copy"

int main() {
    char c = '\0';
    int in = -1, out = -1;

    in = open(INPUT_FILE_NAME, O_RDONLY);

    // If not exist, creates it, if do, truncates it.
    out = open(OUTPUT_FILE_NAME, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR);

    while(read(in, &c, 1) == 1) {
        write(out, &c, 1);
    }

    close(in);
    close(out);
    return 0;
}
