#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "spu.h"
#include "program.h"
#include "Stack/stack.h"

static bool get_two_args (size_t* const first, size_t* const second, stack_t* const stk);

#define CASE_JUMP(jump_cmd, sign)                                                                       \
            case jump_cmd:                                                                              \
            {                                                                                           \
                size_t first  = 0;                                                                      \
                size_t second = 0;                                                                      \
                if (!(get_two_args (&first, &second, &processor.stk)))                                  \
                {                                                                                       \
                    fprintf (stderr, "There is no element in stack to do the command" #jump_cmd "\n");  \
                    free (processor.code);                                                              \
                    fclose  (input);                                                                    \
                    STACK_DTOR (processor.stk);                                                         \
                    return EXIT_FAILURE;                                                                \
                }                                                                                       \
                                                                                                        \
                if (first sign second)                                                                     \
                {                                                                                       \
                    processor.ip = processor.code [processor.ip];                                       \
                }                                                                                       \
                else                                                                                    \
                {                                                                                       \
                    processor.ip++;                                                                     \
                }                                                                                       \
                                                                                                        \
                break;                                                                                  \
            }

int main (const int argc, const char* const argv[])
{
    if (argc != 2)
    {
        fprintf (stderr, "Incorrect number of files for programming\n");
    }

    FILE* input = fopen (argv [1], "r+b");
    if (input == NULL)
    {
        perror ("Can't open file"); // NOTE
        return EXIT_FAILURE;
    }

    spu_t processor = {.count_cmd = 0, .ip = 0, .code = NULL, .stk = NULL};

    fread (&processor.count_cmd, 1, sizeof (processor.count_cmd), input);

    processor.code = (size_t*) calloc (processor.count_cmd, sizeof (size_t));
    if (processor.code == NULL)
    {
        fprintf (stderr, "The program can't calloc memory for code\n");
        fclose (input);
        return EXIT_FAILURE;
    }

    STACK_INIT (stk, processor.count_cmd);

    processor.stk = stk;

    fread (processor.code, sizeof (size_t), processor.count_cmd, input);

    while (processor.ip < processor.count_cmd)
    {
        enum COMMANDS cmd = (enum COMMANDS) processor.code [processor.ip];

        processor.ip++;

        switch (cmd)
        {
            case PUSH:
            {
                size_t number = processor.code [processor.ip];

                processor.ip++;

                stack_push (processor.stk, number);

                break;
            }
            case PUSHR:
            {
                size_t number = processor.code [processor.ip];

                processor.ip++;

                stack_push (processor.stk, processor.regs [number]);

                break;
            }
            case POP:
            {
                size_t number = processor.code [processor.ip];

                processor.ip++;

                if (number == 0)
                {
                    continue;
                }

                if (stack_pop (processor.stk, &(processor.regs [number])) == CANT_POP)
                {
                    fprintf (stderr, "There is no element in stack to do the command\n");
                    free (processor.code);
                    fclose  (input);
                    STACK_DTOR (processor.stk);
                    return EXIT_FAILURE;
                }

                break;
            }
            case ADD:
            {
                size_t first  = 0;
                size_t second = 0;

                if (!(get_two_args (&first, &second, &processor.stk)))
                {
                    fprintf (stderr, "There is no element in stack to do the command\n");
                    free (processor.code);
                    fclose  (input);
                    STACK_DTOR (processor.stk);
                    return EXIT_FAILURE;
                }
                stack_push (processor.stk, first + second);
                break;
            }
            case SUB:
            {
                size_t first  = 0;
                size_t second = 0;
                if (!(get_two_args (&first, &second, &processor.stk)))
                {
                    fprintf (stderr, "There is no element in stack to do the command\n");
                    free (processor.code);
                    fclose  (input);
                    STACK_DTOR (processor.stk);
                    return EXIT_FAILURE;
                }
                stack_push (processor.stk, first - second);
                break;
            }
            case MUL:
            {
                size_t first  = 0;
                size_t second = 0;
                if (!(get_two_args (&first, &second, &processor.stk)))
                {
                    fprintf (stderr, "There is no element in stack to do the command\n");
                    free (processor.code);
                    fclose  (input);
                    STACK_DTOR (processor.stk);
                    return EXIT_FAILURE;
                }
                stack_push (processor.stk, first * second);
                break;
            }
            case DIV:
            {
                size_t first = 0;
                size_t second = 0;
                if (!(get_two_args (&first, &second, &processor.stk)))
                {
                    fprintf (stderr, "There is no element in stack to do the command\n");
                    free (processor.code);
                    fclose  (input);
                    STACK_DTOR (processor.stk);
                    return EXIT_FAILURE;
                }
                if (second == 0)
                {
                    fprintf (stderr, "Can't divide on zero in command\n");
                    free (processor.code);
                    fclose  (input);
                    STACK_DTOR (processor.stk);
                    return EXIT_FAILURE;
                }
                stack_push (processor.stk, first / second);
                break;
            }
            case OUT:
            {
                size_t number = 0;
                if (stack_pop (processor.stk, &number) == CANT_POP)
                {
                    fprintf (stderr, "There is no element in stack to do the command\n");
                    free (processor.code);
                    fclose  (input);
                    STACK_DTOR (processor.stk);
                    return EXIT_FAILURE;
                }
                fprintf (stdout, "%lu\n", number);
                break;
            }
            case DUMP:
            {
                DUMP (processor.stk);
                break;
            }
            case JMP:
            {
                processor.ip = processor.code [processor.ip];
                fprintf (stderr, "ip = %lu\n", processor.ip);
                break;
            }
            CASE_JUMP (JA,  > );
            CASE_JUMP (JAE, >=);
            CASE_JUMP (JB,  < );
            CASE_JUMP (JBE, <=);
            CASE_JUMP (JE,  ==);
            CASE_JUMP (JNE, !=);
            case HLT:
            {
                free (processor.code);
                STACK_DTOR (processor.stk);
                fclose (input);
                return EXIT_SUCCESS;
            }
            default:
            {
                fprintf (stderr, "There is undefined command %lu with number %lu\n", cmd, processor.ip);
                free (processor.code);
                fclose  (input);
                STACK_DTOR (processor.stk);
                return EXIT_FAILURE;
            }
        }
    }

    free (processor.code);
    STACK_DTOR (processor.stk);
    fclose (input);
    return EXIT_SUCCESS;
}

static bool get_two_args (size_t* const first, size_t* const second, stack_t* const stk)
{
    assert (first  != NULL);
    assert (second != NULL);
    assert (stk    != NULL);

    if (stack_pop (*stk, first) == CANT_POP)
    {
        return false;
    }
    if (stack_pop (*stk, second) == CANT_POP)
    {
        return false;
    }

    return true;
}
