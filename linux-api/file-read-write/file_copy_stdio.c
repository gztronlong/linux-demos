
#include <stdio.h>
#include <stdlib.h>

#define INPUT_FILE_NAME     "1m_data.file"
#define OUTPUT_FILE_NAME    "1m_data.file.copy"

int main() {
    int c = 0;
    FILE *pfin = NULL;
    FILE *pfout = NULL;

    pfin = fopen(INPUT_FILE_NAME, "r");
    pfout = fopen(OUTPUT_FILE_NAME, "w");

    while(fread(&c, sizeof(char), 1, pfin)) {
        fwrite(&c, sizeof(char), 1, pfout);
    }

    fclose(pfin);
    fclose(pfout);
    return 0;
}
