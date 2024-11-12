#include "spu.h"

#include <stdlib.h>
#include <stdio.h>

#include "../My_lib/Logger/logging.h"
#include "../My_lib/My_stdio/my_stdio.h"

#define ERROR_HANDLER(error)                                                                            \
    if (error != DONE_SPU)                                                                              \
    {                                                                                                   \
        fprintf (stderr, "Code error = {%d} with "                                                      \
                         "command number = [%lu] and ip = [%lu]\n",                                     \
                         error, processor.count_cmd, processor.ip);                                     \
        spu_dtor (&processor);                                                                          \
        return EXIT_FAILURE;                                                                            \
    }

int main (const int argc, const char* argv[])
{
    if (argc != 2)
    {
        fprintf (stderr, "Incorrect number of files for processing.\n"
                         "You need to write the name of input file.\n");

        return EXIT_FAILURE;
    }

    FILE* const error_file = fopen ("My_lib/Logger/error.txt", "w");
    if (error_file == NULL)
    {
        fprintf (stderr, "Can't start logging\n");
        return EXIT_FAILURE;
    }
    set_log_file (error_file);
    set_log_lvl (DEBUG);

    spu_t processor = {0};

    enum SPU_ERROR result = spu_ctor (&processor, argv [1]);

    ERROR_HANDLER (result);

    result = processing (&processor);

    ERROR_HANDLER (result);

    result = spu_dtor (&processor);

    ERROR_HANDLER (result);

    fclose (error_file);

    return EXIT_SUCCESS;
}

#undef ERROR_HANDLER

