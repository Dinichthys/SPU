#include <stdlib.h>
#include <stdio.h>

#include "assembler.h"
#include "../Logger/logging.h"

#define ERROR_HANDLER(error)                                                                            \
    if (error != DONE_ASM)                                                                              \
    {                                                                                                   \
        fprintf (stderr, "Code error = {%d} with "                                                      \
                         "command number = [%lu] and input offset = [%lu]\n",                           \
                         error, assembler.count_cmd, assembler.input_offset);                           \
        asm_dtor (&assembler);                                                                          \
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
        fprintf (stderr, "Incorrect number of files for compiling\n"
                         "You need to write the name of input file firstly.\n"
                         "Secondly you must write the name of output file.\n");

        fclose (error_file);
        return EXIT_FAILURE;
    }

    // NOTE зануление в корнструктор
    assembler_t assembler = {.labels = {}, .labels_size = 0,
                             .code = NULL, .count_cmd = 0,
                             .input_buffer = NULL, .input_offset = 0,
                             .output = argv [2]};

    // NOTE не передавать argv, че за нах
    enum ASSEMBLER_ERROR result = asm_ctor (&assembler, argv [1]);

    ERROR_HANDLER (result);

    result = compile (&assembler);

    ERROR_HANDLER (result);

    assembler.count_cmd = 0;
    assembler.input_offset = 0;

    result = compile (&assembler);

    ERROR_HANDLER (result);

    bool program_success = true;

    for (size_t i = 0; i < assembler.labels_size; i++)
    {
        if ((size_t) -1 == (assembler.labels[i].pointer))
        {
            program_success = false;
            fprintf (stderr, "Undefined label %s\n", assembler.labels[i].name);
        }
    }

    result = write_result (&assembler);

    ERROR_HANDLER (result);

    result = asm_dtor (&assembler);

    ERROR_HANDLER (result);

    fclose   (error_file);

    return (program_success) ? EXIT_SUCCESS : EXIT_FAILURE;
}
