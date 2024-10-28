#include "spu.h"

#include <stdlib.h>
#include <stdio.h>

#include "../Logger/logging.h"
#include "../My_stdio/my_stdio.h"

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
    FILE* const error_file = fopen ("error.txt", "w");
    if (error_file == NULL)
    {
        fprintf (stderr, "Can't start logging\n");
        return EXIT_FAILURE;
    }
    set_log_file (error_file);
    set_log_lvl (DEBUG);

    if (argc != 2)
    {
        fprintf (stderr, "Incorrect number of files for programming\n");
        fclose (error_file);
        return EXIT_FAILURE;
    }

    spu_t processor = {.count_cmd = 0, .ip = 0, .code = NULL, .regs = {0},
                       .stk = 0, .stack_func_call_ip = 0, .ram = NULL};

    enum SPU_ERROR result = spu_ctor (&processor, argv [1]);

    ERROR_HANDLER (result);

    result = processing (&processor);

    ERROR_HANDLER (result);

    result = spu_dtor (&processor);

    ERROR_HANDLER (result);

    fclose (error_file);

    return EXIT_SUCCESS;
}

