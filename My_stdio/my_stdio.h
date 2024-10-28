#ifndef MY_STDIO_H
#define MY_STDIO_H

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

// NOTE вынести СУКА!!!
int my_strcmp (const char* const s1, const char* const s2);
size_t skip_space_symbols (char* const string, const size_t offset);
size_t size_of_file (FILE* const input);

#endif // MY_STDIO_H
