#include <stdlib.h>
#include <stdio.h>

#include "spu.h"

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
        fprintf (stderr, "Incorrect number of files for programming\n");
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

    return EXIT_SUCCESS;
}

