#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "program.h"
#include "assembler.h"
#include "Assert/my_assert.h"
#include "Logger/logging.h"

static const int NAME_REG       = 5;
static const int COUNT_REGS     = 10;
static const int NUMBER_LABELS  = 100;
static const int LABEL_NAME     = 100;

static bool jumper (char* const cmd, assembler_t* const assembler, FILE* const input);
static int my_strcmp (const char* const s1, const char* const s2);
static int register_num (char name [NAME_REG]);
static size_t label_num (char label [LABEL_NAME], assembler_t* const assembler);

#define CASE_CMD(variety)                                       \
    if (my_strcmp (cmd, #variety) == 0)                         \
    {                                                           \
        assembler->code [assembler->count_cmd - 1] = variety;   \
        continue;                                               \
    }

long compile (assembler_t* const assembler, FILE* const input)
{
    ASSERT (assembler != NULL, "Assembler = NULL in compiler\n");
    ASSERT (input != NULL, "Input file = NULL in compiler\n");

    while (true)
    {
        char cmd [30] = "";

        if (fscanf (input, "%s", cmd) == EOF)
        {
            break;
        }

        (assembler->count_cmd)++;

        if (strcmp (cmd, "push") == 0)
        {
            int number = 0;
            if (fscanf (input, "%d", &number) != 1)
            {
                char reg [NAME_REG] = "";

                if (fscanf (input, "%s", reg) == EOF)
                {
                    fprintf (stderr, "Push hasn't an argument at the position with count_cmd = %lu\n", assembler->count_cmd);
                    return EXIT_FAILURE;
                }

                assembler->code [assembler->count_cmd - 1] = PUSHR;
                (assembler->count_cmd)++;

                int reg_num = register_num (reg);
                if (reg_num == -1)
                {
                    fprintf (stderr, "Push has invalid name for register\n");
                    return EXIT_FAILURE;
                }

                assembler->code [assembler->count_cmd - 1] = (size_t) reg_num;
                continue;
            }
            assembler->code [assembler->count_cmd - 1] = PUSH;
            (assembler->count_cmd)++;
            assembler->code [assembler->count_cmd - 1] = (size_t)number;
            continue;
        }

        if (my_strcmp (cmd, "pop") == 0)
        {
            assembler->code [assembler->count_cmd - 1] = POP;

            (assembler->count_cmd)++;

            char reg [NAME_REG] = "";

            if (fscanf (input, "%s", reg) == EOF)
            {
                fprintf (stderr, "Pop hasn't an argument at the position with count_cmd = %lu\n", assembler->count_cmd);
                return EXIT_FAILURE;
            }

            int reg_num = register_num (reg);
            if (reg_num == -1)
            {
                fprintf (stderr, "Pop has invalid name for register\n");
                return EXIT_FAILURE;
            }

            assembler->code [assembler->count_cmd - 1] = (size_t) reg_num;
            continue;
        }

        CASE_CMD (ADD);
        CASE_CMD (SUB);
        CASE_CMD (MUL);
        CASE_CMD (DIV);
        CASE_CMD (OUT);
        CASE_CMD (DUMP);

        if (jumper (cmd, assembler, input))
        {
            continue;
        }

        CASE_CMD (HLT);

        if (cmd [strlen (cmd) - 1] == ':')
        {
            assembler->labels [assembler->labels_size].pointer = assembler->count_cmd - 1;
            strcpy (assembler->labels [assembler->labels_size].name, cmd);
            (assembler->labels_size)++;
            (assembler->count_cmd)--;
            continue;
        }

        fprintf (stderr, "Invalid name for command: %s in input file with count_cmd = %lu\n", cmd, assembler->count_cmd);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

static bool jumper (char* const cmd, assembler_t* const assembler, FILE* const input)
{
    ASSERT (cmd != NULL, "Invalid pointer for cmd in jumper\n");
    ASSERT (assembler != NULL, "Invalid pointer for count_cmd in jumper\n");
    ASSERT (input != NULL, "Invalid pointer for code in jumper\n");

    #define CASE_JUMP(jump_cmd)                                                             \
        if (my_strcmp (cmd, #jump_cmd) == 0)                                                \
        {                                                                                   \
            assembler->code [assembler->count_cmd - 1] = jump_cmd;                          \
            (assembler->count_cmd)++;                                                       \
            char label [LABEL_NAME] = "";                                                   \
            if (fscanf (input, "%s", label) == EOF)                                         \
            {                                                                               \
                return false;                                                               \
            }                                                                               \
            assembler->code [assembler->count_cmd - 1] = label_num (label, assembler);      \
                                                                                            \
            return true;                                                                    \
        }

    LOG (DEBUG, "The function jumper got arguments:\n"
                "| cmd = %p | assembler = %p | input = %p |\n", cmd, assembler, input);

    CASE_JUMP (JMP);
    CASE_JUMP (JA);
    CASE_JUMP (JAE);
    CASE_JUMP (JB);
    CASE_JUMP (JBE);
    CASE_JUMP (JE);
    CASE_JUMP (JNE);

    #undef CASE_JUMP

    return false;
}

static int my_strcmp (const char* const s1, const char* const s2)
{
    ASSERT (s1 != NULL, "Invalid pointer s1 for function %s\n", __FUNCTION__);
    ASSERT (s2 != NULL, "Invalid pointer s2 for function %s\n", __FUNCTION__);

    LOG (DEBUG, "Function %s got arguments:\n"
                "s1 = %p s2 = %p\n", __FUNCTION__, s1, s2);

    size_t i = 0;
    while ((tolower (s1 [i]) - tolower (s2 [i])) == 0)
    {
        if (s1 [i] == '\0')
        {
            return 0;
        }
        i++;
    }

    return (s1 [i] - s2 [i]);
}

static int register_num (char name [NAME_REG])
{
    ASSERT (name != NULL, "Invalid pointer name for function %s\n", __FUNCTION__);

    LOG (DEBUG, "Function %s got argument:\n"
                "name = %p", __FUNCTION__, name);

    const char* regs [COUNT_REGS] = {"ax", "bx", "cx", "dx", "ex", "fx", "gx", "hx", "ix", "jx"};

    for (int number = 0; number < COUNT_REGS; number++)
    {
        if (my_strcmp (name, regs [number]) == 0)
        {
            return number;
        }
    }

    return -1;
}

static size_t label_num (char label [LABEL_NAME], assembler_t* const assembler)
{
    ASSERT (label != NULL, "Invalid pointer label for function %s\n", __FUNCTION__);

    LOG (DEBUG, "Function %s got argument:\n"
                "label = %p", __FUNCTION__, label);

    size_t number = 0;

    for (; number < assembler->labels_size; number++)
    {
        if (my_strcmp (assembler->labels [number].name, label) == 0)
        {
            return assembler->labels [number].pointer;
        }
    }

    if (number < NUMBER_LABELS - 1)
    {
        assembler->labels [number].pointer = (size_t) -1;
        strcpy (assembler->labels [number].name, label);
        (assembler->labels_size)++;
    }

    return (size_t) -1;
}
