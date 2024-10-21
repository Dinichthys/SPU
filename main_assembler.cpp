#include <stdlib.h>
#include <stdio.h>

#include "assembler.h"
#include "Logger/logging.h"

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

    if (argc != 3)
    {
        fprintf (stderr, "Incorrect number of files for programming\n");
    }

    FILE* input = NULL;
    input = fopen (argv [1], "r");
    if (input == NULL)
    {
        fprintf (stderr, "Can't open file %s\n", argv [1]);
        fclose (error_file);
        return EXIT_FAILURE;
    }

    FILE* output = NULL;
    output = fopen (argv [2], "w+b");
    if (output == NULL)
    {
        fprintf (stderr, "Can't open file %s\n", argv [2]);
        fclose  (input);
        fclose (error_file);
        return EXIT_FAILURE;
    }

    assembler_t assembler = {.labels = {}, .labels_size = 0, .code = NULL, .count_cmd = 0};

    fseek (input, 0, SEEK_END);
    size_t size_code = ftell (input);
    fseek (input, 0, SEEK_SET);

    assembler.code = (size_t*) calloc (size_code, sizeof (size_t));

    if (assembler.code == NULL)
    {
        fprintf (stderr, "The program can't calloc\n");
        fclose (input);
        fclose (output);
        fclose (error_file);
        return EXIT_FAILURE;
    }

    long result = compile (&assembler, input);

    if (result == EXIT_FAILURE)
    {
        fprintf (stderr, "The program can't compile for the first time\n");
        free   (assembler.code);
        fclose (input);
        fclose (output);
        fclose (error_file);
        return result;
    }

    fseek (input, 0, SEEK_SET);
    assembler.count_cmd = 0;

    result = compile (&assembler, input);

    if (result == EXIT_FAILURE)
    {
        fprintf (stderr, "The program can't compile for the second time\n");
        free   (assembler.code);
        fclose (input);
        fclose (output);
        fclose (error_file);
        return result;
    }

    bool program_success = true;

    for (size_t i = 0; i < assembler.labels_size; i++)
    {
        if ((size_t) -1 == (assembler.labels[i].pointer))
        {
            program_success = false;
            fprintf (stderr, "Undefined label %s\n", assembler.labels[i].name);
        }
    }

    fwrite (&(assembler.count_cmd), 1, sizeof (assembler.count_cmd), output);
    fwrite (assembler.code, sizeof (size_t), assembler.count_cmd, output);

    free   (assembler.code);
    fclose (input);
    fclose (output);
    fclose (error_file);

    return (program_success) ? EXIT_SUCCESS : EXIT_FAILURE;
}
