#include "my_stdio.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "../Logger/logging.h"
#include "../Assert/my_assert.h"

// NOTE вынести СУКА!!!
int my_strcmp (const char* const s1, const char* const s2)
{
    ASSERT (s1 != NULL, "Invalid pointer s1 for function %s\n", __FUNCTION__);
    ASSERT (s2 != NULL, "Invalid pointer s2 for function %s\n", __FUNCTION__);

    LOG (DEBUG, "Function got arguments:\n"
                "s1 = %p s2 = %p\n", s1, s2);

    size_t i = 0;
    while ((tolower (s1 [i]) - tolower (s2 [i])) == 0)
    {
        if (s1 [i] == '\0')
        {
            return 0;
        }
        i++;
    }

    return (tolower (s1 [i]) - tolower (s2 [i]));
}

size_t skip_space_symbols (char* const string, const size_t offset)
{
    ASSERT (string != NULL, "Invalid argument for [%s]: string = %p\n", __FUNCTION__, string);

    size_t new_offset = offset;
    char check = '\0';

    while ((sscanf (string + new_offset, "%c", &check) != EOF) && (isspace (string [new_offset])))
    {
        new_offset++;
    }

    return new_offset;
}

// NOTE это что за пиздец почему статик между внешними функция
size_t size_of_file (FILE* const input)
{
    ASSERT (input != NULL, "Invalid argument for function size_of_file\n");

    fseek (input, 0, SEEK_END);
    size_t size_code = (size_t) ftell (input);
    fseek (input, 0, SEEK_SET);

    return size_code;
}

