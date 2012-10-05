#include "file.h"

#include <stdio.h>

int create_empty_file(const char *name, int size) {
    FILE *fp = fopen(name, "wb");
    if (fp == NULL) {
        return -1;
    }
    fseek(fp, size-1, SEEK_SET);
    char end = EOF;
    fwrite(&end, sizeof(char), sizeof(end), fp);
    fclose(fp);
    return 0;
}
