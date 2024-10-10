#include <stdio.h>
#include <stdlib.h>
#include "Stack/stack.h"

int main (const int argc, const char* argv[])
{
    if (argc != 2)
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

    STACK_INIT (stk, 100);

    size_t count_cmd = 0;

    while (true)
    {
        long long cmd = 0;
        if (fscanf (input, "%lld", &cmd) != 1)
        {
            fprintf (stderr, "There is invalid program at the position %s:%lu\n", argv [1], count_cmd);
            fclose  (input);
            STACK_DTOR (stk);
            return EXIT_FAILURE;
        }
        count_cmd++;

        switch (cmd)
        {
            case 1:
            {
                long long number = 0;
                if (fscanf (input, "%lld", &number) != 1)
                {
                    fprintf (stderr, "There is no argument for push at the position %s:%lu\n", argv [1], count_cmd);
                    fclose  (input);
                    STACK_DTOR (stk);
                    return EXIT_FAILURE;
                }
                stack_push (stk, number);
                break;
            }
            case 2:
            {
                long long number = 0;
                if (stack_pop (stk, &number) == CANT_POP)
                {
                    fprintf (stderr, "There is no element in stack to do the command %s:%lu\n", argv [1], count_cmd);
                    fclose  (input);
                    STACK_DTOR (stk);
                    return EXIT_FAILURE;
                }
                break;
            }
            case 3:
            {
                long long a = 0;
                if (stack_pop (stk, &a) == CANT_POP)
                {
                    fprintf (stderr, "There is no element in stack to do the command %s:%lu\n", argv [1], count_cmd);
                    fclose  (input);
                    STACK_DTOR (stk);
                    return EXIT_FAILURE;
                }
                long long b = 0;
                if (stack_pop (stk, &b) == CANT_POP)
                {
                    fprintf (stderr, "There is no element in stack to do the command %s:%lu\n", argv [1], count_cmd);
                    fclose  (input);
                    STACK_DTOR (stk);
                    return EXIT_FAILURE;
                }
                stack_push (stk, a + b);
                break;
            }
            case 4:
            {
                long long a = 0;
                if (stack_pop (stk, &a) == CANT_POP)
                {
                    fprintf (stderr, "There is no element in stack to do the command %s:%lu\n", argv [1], count_cmd);
                    fclose  (input);
                    STACK_DTOR (stk);
                    return EXIT_FAILURE;
                }
                long long b = 0;
                if (stack_pop (stk, &b) == CANT_POP)
                {
                    fprintf (stderr, "There is no element in stack to do the command %s:%lu\n", argv [1], count_cmd);
                    fclose  (input);
                    STACK_DTOR (stk);
                    return EXIT_FAILURE;
                }
                stack_push (stk, a - b);
                break;
            }
            case 5:
            {
                long long a = 0;
                if (stack_pop (stk, &a) == CANT_POP)
                {
                    fprintf (stderr, "There is no element in stack to do the command %s:%lu\n", argv [1], count_cmd);
                    fclose  (input);
                    STACK_DTOR (stk);
                    return EXIT_FAILURE;
                }
                long long b = 0;
                if (stack_pop (stk, &b) == CANT_POP)
                {
                    fprintf (stderr, "There is no element in stack to do the command %s:%lu\n", argv [1], count_cmd);
                    fclose  (input);
                    STACK_DTOR (stk);
                    return EXIT_FAILURE;
                }
                stack_push (stk, a * b);
                break;
            }
            case 6:
            {
                long long a = 0;
                if (stack_pop (stk, &a) == CANT_POP)
                {
                    fprintf (stderr, "There is no element in stack to do the command %s:%lu\n", argv [1], count_cmd);
                    fclose  (input);
                    STACK_DTOR (stk);
                    return EXIT_FAILURE;
                }
                long long b = 0;
                if (stack_pop (stk, &b) == CANT_POP)
                {
                    fprintf (stderr, "There is no element in stack to do the command %s:%lu\n", argv [1], count_cmd);
                    fclose  (input);
                    STACK_DTOR (stk);
                    return EXIT_FAILURE;
                }
                if (b == 0)
                {
                    fprintf (stderr, "Can't divide on zero in command %s:%lu\n", argv [1], count_cmd);
                    fclose  (input);
                    STACK_DTOR (stk);
                    return EXIT_FAILURE;
                }
                stack_push (stk, a / b);
                break;
            }
            case 7:
            {
                long long number = 0;
                if (stack_pop (stk, &number) == CANT_POP)
                {
                    fprintf (stderr, "There is no element in stack to do the command %s:%lu\n", argv [1], count_cmd);
                    fclose  (input);
                    STACK_DTOR (stk);
                    return EXIT_FAILURE;
                }
                fprintf (stdout, "%lld\n", number);
                break;
            }
            case 8:
            {
                DUMP (stk);
                break;
            }
            case -1:
            {
                fclose (input);
                STACK_DTOR (stk);
                return EXIT_SUCCESS;
            }
            default:
            {
                fprintf (stderr, "There is undefined command %s:%lu\n", argv [1], count_cmd);
                fclose  (input);
                STACK_DTOR (stk);
                return EXIT_FAILURE;
            }
        }
    }
    fclose (input);
    STACK_DTOR (stk);
    return EXIT_SUCCESS; // atexit (void (*function) (void))
}
