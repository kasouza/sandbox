#include "utils.h"

#include <stdio.h>
#include <stdlib.h>

char *box_read_file(const char *path)
{
    FILE *file_pointer = fopen(path, "r");
    if (!file_pointer)
    {
        BOX_LOG("Could not open file: \"%s\"\n", path);
        exit(-1);
    }

    fseek(file_pointer, 0L, SEEK_END);
    long size = ftell(file_pointer);
    fseek(file_pointer, 0L, SEEK_SET);

    char *file_content = malloc(size + 1);
    fread(file_content, sizeof(char), size, file_pointer);

    file_content[size] = '\0';

    return file_content;
}
