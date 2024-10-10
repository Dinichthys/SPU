#include <stdio.h>
#include <stdlib.h>

#include "assembler.h"
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
        enum ASSEMBLER cmd = 0;
        if (fscanf (input, "%d", &cmd) != 1)
        {
            fprintf (stderr, "There is invalid program at the position %s:%lu\n", argv [1], count_cmd);
            fclose  (input);
            STACK_DTOR (stk);
            return EXIT_FAILURE;
        }
        count_cmd++;

        switch (cmd)
        {
            case PUSH:
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
            case POP:
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
            case ADD:
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
            case SUB:
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
            case MUL:
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
            case DIV:
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
            case OUT:
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
            case DUMP:
            {
                DUMP (stk);
                break;
            }
            case HLT:
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
