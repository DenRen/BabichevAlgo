// Challange: сдать с первого раза

#include <cstdio>
#include "debug_func.h"

long
get_unread_size (FILE* file) {
    long begin = ftell (file);
    CHECK (begin != -1);

    CHECK_ERR (fseek (file, 0, SEEK_END));
    long end = ftell (file);
    CHECK (end);

    CHECK_ERR (fseek (file, begin, SEEK_SET));

    return end - begin;
}

int main () {
    const unsigned maxRamSize = 256 * 1024;
    const char input_file_name[]  = "input.txt";
    const char output_file_name[] = "output.txt";

    FILE* input_file = fopen (input_file_name, "r");
    CHECK_PTR (input_file);

    long input_file_size = get_unread_size (input_file);
    CHECK (input_file_size != -1);

    printf ("size: %ld\n", input_file_size);

    CHECK_ERR (fclose (input_file));
}