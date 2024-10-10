#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler.h"

int main (const int argc, const char* argv [])
{
    if (argc != 3)
    {
        fprintf (stderr, "Incorrect number of files for programming\n");
    }

    FILE* input = NULL;
    input = fopen (argv [1], "r");
    if (input == NULL)
    {
        fprintf (stderr, "Can't open file %s\n", argv [1]);
        return EXIT_FAILURE;
    }

    FILE* output = NULL;
    output = fopen (argv [2], "w");
    if (output == NULL)
    {
        fprintf (stderr, "Can't open file %s\n", argv [2]);
        fclose  (input);
        return EXIT_FAILURE;
    }

    size_t count_cmd = 0;

    while (true)
    {
        char cmd [30] = "";
        if (fscanf (input, "%s", cmd) == EOF)
        {
            fprintf (stderr, "The program hasn't end-command\n");
            fclose (input);
            fclose (output);
            return EXIT_FAILURE;
        }
        count_cmd++;
        if (strcmp (cmd, "push") == 0)
        {
            fprintf (output, "%d ", PUSH);
            long long number = 0;
            if (fscanf (input, "%lld", &number) != 1)
            {
                fprintf (stderr, "Push hasn't an argument at the position %s:%lu\n", argv [1], count_cmd);
                fclose (input);
                fclose (output);
                return EXIT_FAILURE;
            }
            fprintf (output, "%lld\n", number);
            continue;
        }
        if (strcmp (cmd, "pop") == 0)
        {
            fprintf (output, "%d\n", POP);
            continue;
        }
        if (strcmp (cmd, "add") == 0)
        {
            fprintf (output, "%d\n", ADD);
            continue;
        }
        if (strcmp (cmd, "sub") == 0)
        {
            fprintf (output, "%d\n", SUB);
            continue;
        }
        if (strcmp (cmd, "mul") == 0)
        {
            fprintf (output, "%d\n", MUL);
            continue;
        }
        if (strcmp (cmd, "div") == 0)
        {
            fprintf (output, "%d\n", DIV);
            continue;
        }
        if (strcmp (cmd, "out") == 0)
        {
            fprintf (output, "%d\n", OUT);
            continue;
        }
        if (strcmp (cmd, "dump") == 0)
        {
            fprintf (output, "%d\n", DUMP);
            continue;
        }
        if (strcmp (cmd, "hlt") == 0)
        {
            fprintf (output, "%d\n", HLT);
            break;
        }
        fprintf (stderr, "Invalid name for command: %s in file %s:%lu\n", cmd, argv [1], count_cmd);
        fclose (input);
        fclose (output);
        return EXIT_FAILURE;
    }

    fclose (input);
    fclose (output);

    return EXIT_SUCCESS;
}
