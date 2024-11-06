#include "disassembler.h"

#include <stdio.h>
#include <stdlib.h>

#include "../Logger/logging.h"

#define ERROR_HANDLER(error)                                                                            \
    if (error != DONE_DISASM)                                                                           \
    {                                                                                                   \
        fprintf (stderr, "Code error = {%d} with "                                                      \
                         "command number = [%lu] and ip = [%lu]\n",                                     \
                         error, disassembler.count_cmd, disassembler.ip);                               \
        disasm_dtor (&disassembler);                                                                    \
        fclose (error_file);                                                                            \
        return EXIT_FAILURE;                                                                            \
    }

int main (const int argc, const char* argv[])
{
    FILE* const error_file = fopen ("Logger/error.txt", "w");
    if (error_file == NULL)
    {
        fprintf (stderr, "Can't start logging\n");
        return EXIT_FAILURE;
    }
    set_log_file (error_file);
    set_log_lvl (DEBUG);

    if (argc != 3)
    {
        fprintf (stderr, "Incorrect number of files for disassembling.\n"
                         "You need to write the name of input file firstly.\n"
                         "Secondly you must write the name of output file.\n");

        return EXIT_FAILURE;
    }

    disassembler_t disassembler = {0};

    enum DISASSEMBLER_ERROR result = disasm_ctor (&disassembler, argv [1]);

    ERROR_HANDLER (result);

    result = disassembling (&disassembler);

    ERROR_HANDLER (result);

    result = write_result (&disassembler, argv [2]);

    ERROR_HANDLER (result);

    result = disasm_dtor (&disassembler);

    ERROR_HANDLER (result);

    fclose (error_file);

    return EXIT_SUCCESS;
}
