#ifndef MY_STDIO_H
#define MY_STDIO_H

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#include "../Logger/logging.h"
#include "../Assert/my_assert.h"

// NOTE вынести СУКА!!!
int my_strcmp (const char* const s1, const char* const s2);
size_t skip_space_symbols (char* const string, const size_t offset);
size_t size_of_file (FILE* const input);

#define FOPEN(file, name, mode)                                                     \
{                                                                                   \
    file = fopen (name, mode);                                                      \
    LOG (DEBUG, "File with name \"%s\" and pointer [%p] was opened\n", name, file); \
}

#define FCLOSE(file)                                            \
{                                                               \
    LOG (DEBUG, "File with pointer [%p] was closed\n", file);   \
    fclose (file);                                              \
    file = NULL;                                                \
}

#endif // MY_STDIO_H
