#include "assembler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../program.h"
#include "../My_stdio/my_stdio.h"
#include "../Assert/my_assert.h"
#include "../Logger/logging.h"

static int register_num (char name [REG_NAME_LEN]);
static size_t label_num (char label [LABEL_NAME_LEN], assembler_t* const assembler);
static enum ASSEMBLER_ERROR push_cmd (assembler_t* const assembler);
static enum ASSEMBLER_ERROR pop_cmd (assembler_t* const assembler);
static bool jumper (char* const cmd, assembler_t* const assembler);

#define CASE_CMD(variety)                                                                                   \
    if (my_strcmp (cmd, #variety) == 0)                                                                     \
    {                                                                                                       \
        assembler->code [assembler->count_cmd - 1] = (command_t) (((command_t) variety) << ARGUMENT_TYPE);  \
        continue;                                                                                           \
    }

enum ASSEMBLER_ERROR compile (assembler_t* const assembler)
{
    ASSERT (assembler != NULL, "Assembler = NULL in compiler\n");

    while (true)
    {
        char cmd [CMD_NAME_LEN] = "";

        if (sscanf (assembler->input_buffer + assembler->input_offset, "%s", cmd) == EOF)
        {
            break;
        }

        if (cmd [0] == COMMENT_SYMBOL)
        {
            char* comment_end = strchr (assembler->input_buffer + assembler->input_offset, '\n');

            if (comment_end == NULL)
            {
                break;
            }

            LOG (DEBUG, "Offset = %lu \n"
                        "Pointer on the end of comment = %p \n"
                        "Pointer on the buffer = %p\n"
                        "New offset = %lu\n"
                        "Char code = %d\n",
                        assembler->input_offset,
                        comment_end, assembler->input_buffer,
                        comment_end - assembler->input_buffer + 1,
                        *comment_end);

            assembler->input_offset = comment_end - assembler->input_buffer + 1;

            LOG (DEBUG, "Commentation %s with offset %lu\n", cmd, assembler->input_offset);

            continue;
        }

        (assembler->input_offset) += strlen (cmd);

        assembler->input_offset = skip_space_symbols (assembler->input_buffer, assembler->input_offset);

        (assembler->count_cmd)++;

        if (my_strcmp (cmd, "push") == 0)
        {
            enum ASSEMBLER_ERROR result = push_cmd (assembler);
            if (result != DONE_ASM)
            {
                return result;
            }
            continue;
        }

        if (my_strcmp (cmd, "pop") == 0)
        {
            enum ASSEMBLER_ERROR result = pop_cmd (assembler);
            if (result != DONE_ASM)
            {
                return result;
            }
            continue;
        }

        CASE_CMD (ADD);
        CASE_CMD (SUB);
        CASE_CMD (MUL);
        CASE_CMD (DIV);
        CASE_CMD (SQRT);
        CASE_CMD (IN);
        CASE_CMD (OUT);
        CASE_CMD (DUMP);
        CASE_CMD (RET);
        CASE_CMD (DRAW);

        if (jumper (cmd, assembler))
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

        return INVAL_COMMAND;
    }

    return DONE_ASM;
}

#undef CASE_CMD

static int register_num (char name [REG_NAME_LEN])
{
    ASSERT (name != NULL, "Invalid pointer name for function %s\n", __FUNCTION__);

    LOG (DEBUG, "Function %s got argument:\n"
                "name = %p", __FUNCTION__, name);

    for (int number = 0; number < COUNT_REGS; number++)
    {
        if (my_strcmp (name, REGISTERS [number]) == 0)
        {
            return number;
        }
    }

    return -1;
}

static size_t label_num (char label [LABEL_NAME_LEN], assembler_t* const assembler)
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

    return (size_t) -1;
}

enum ASSEMBLER_ERROR asm_ctor (assembler_t* const assembler, const char* name_input)
{
    ASSERT (assembler != NULL,
            "Invalid argument for constructor of the assembler (asm) with pointer %p\n", assembler);
    ASSERT (name_input != NULL,
            "Invalid argument for constructor of the assembler (name_input) with pointer %p\n", name_input);

    memset (assembler->labels, 0, NUMBER_LABELS * sizeof (assembler->labels [0]));
    assembler->labels_size = 0;

    FILE* input = NULL;
    FOPEN (input, name_input, "r");
    if (input == NULL)
    {
        return CANT_CTOR_ASM;
    }

    size_t size_code = 3 * size_of_file (input);

    assembler->code = (command_t*) calloc (size_code, sizeof (size_t));
    if (assembler->code == NULL)
    {
        FCLOSE (input);
        return CANT_CTOR_ASM;
    }

    assembler->count_cmd = 0;

    assembler->input_buffer = (char*) calloc (size_code, sizeof (char));
    if (assembler->input_buffer == NULL)
    {
        FCLOSE (input);
        return CANT_CTOR_ASM;
    }

    fread (assembler->input_buffer, sizeof (char), size_code, input);
    FCLOSE (input);

    assembler->input_offset = 0;

    return DONE_ASM;
}

enum ASSEMBLER_ERROR asm_dtor (assembler_t* const assembler)
{
    ASSERT (assembler != NULL,
            "Invalid argument for destructor of the assembler with pointer %p\n", assembler);

    assembler->labels_size = 0;
    free (assembler->code);
    assembler->count_cmd = 0;
    free (assembler->input_buffer);
    assembler->input_offset = 0;

    return DONE_ASM;
}

enum ASSEMBLER_ERROR write_result (assembler_t* const assembler)
{
    ASSERT (assembler != NULL, "Invalid argument for write_result assembler = %p\n", assembler);

    FILE* output_file = NULL;

    FOPEN (output_file, assembler->output, "w+b");
    if (assembler->output == NULL)
    {
        return CANT_WRITE_RESULT_ASM;
    }

    fwrite (&(assembler->count_cmd), 1, sizeof (assembler->count_cmd), output_file);
    fwrite (assembler->code, sizeof (command_t), assembler->count_cmd, output_file);

    FCLOSE (output_file);

    return DONE_ASM;
}

static enum ASSEMBLER_ERROR push_cmd (assembler_t* const assembler)
{
    ASSERT (assembler != NULL,
            "Invalid argument for push_cmd of the assembler with pointer %p\n", assembler);

    command_t argument = (((command_t) PUSH) << ARGUMENT_TYPE);

    double number = 0;

    char str_argument [STR_ARGUMENT_LEN] = "";
    size_t arg_offset = 0;

    sscanf (assembler->input_buffer + assembler->input_offset, "%s", str_argument);
    assembler->input_offset += strlen (str_argument);
    assembler->input_offset = skip_space_symbols (assembler->input_buffer, assembler->input_offset);

    char reg [REG_NAME_LEN] = "";
    size_t number_reg = 0;
    char* plus_position = strchr (str_argument, '+');

    if (str_argument [0] == '[')
    {
        argument |= RAM;

        arg_offset++;
        str_argument [strlen (str_argument) - 1] = '\0';
    }

    if (sscanf (str_argument + arg_offset, "%lf", &number) == 1)
    {
        argument |= IMMED_NUM;

        memcpy (assembler->code + assembler->count_cmd - 1, &argument, sizeof (argument));

        memcpy (assembler->code + assembler->count_cmd, &number, sizeof (number));
        assembler->count_cmd += sizeof (number);

        return DONE_ASM;
    }

    if (plus_position != NULL)
    {
        argument |= IMMED_NUM;
        argument |= REGISTER;

        strtok (str_argument, "+");
        sscanf (str_argument + arg_offset, "%s", reg);
        sscanf (plus_position + 1, "%lf", &number);

        memcpy (assembler->code + assembler->count_cmd - 1, &argument, sizeof (argument));

        number_reg = (size_t) register_num (reg);
        if (number_reg == (size_t) -1)
        {
            return INVAL_REG;
        }

        memcpy (assembler->code + assembler->count_cmd, &number_reg, sizeof (number_reg));
        assembler->count_cmd += sizeof (number_reg);

        memcpy (assembler->code + assembler->count_cmd, &number, sizeof (number));
        assembler->count_cmd += sizeof (number);

        return DONE_ASM;
    }

    if (sscanf (str_argument + arg_offset, "%s", reg) == 1)
    {
        argument |= REGISTER;

        memcpy (assembler->code + assembler->count_cmd - 1, &argument, sizeof (argument));

        number_reg = (size_t) register_num (reg);
        if (number_reg == (size_t) -1)
        {
            return INVAL_REG;
        }

        memcpy (assembler->code + assembler->count_cmd, &number_reg, sizeof (number_reg));
        assembler->count_cmd += sizeof (number_reg);

        return DONE_ASM;
    }

    return PUSH_NO_ARG;
}

// NOTE мб обобщить push pop, да есть же
static enum ASSEMBLER_ERROR pop_cmd (assembler_t* const assembler)
{
    ASSERT (assembler != NULL,
            "Invalid argument for pop_cmd of the assembler with pointer %p\n", assembler);

    command_t argument = (((command_t) POP) << ARGUMENT_TYPE);

    size_t number = 0;

    char str_argument [STR_ARGUMENT_LEN] = "";
    size_t arg_offset = 0;

    sscanf (assembler->input_buffer + assembler->input_offset, "%s", str_argument);
    assembler->input_offset += strlen (str_argument);
    assembler->input_offset = skip_space_symbols (assembler->input_buffer, assembler->input_offset);

    char reg [REG_NAME_LEN] = "";
    size_t number_reg = 0;
    char* plus_position = strchr (str_argument, '+');

    if (str_argument [0] == '[')
    {
        argument |= RAM;

        arg_offset++;
        str_argument [strlen (str_argument) - 1] = '\0';
    }

    if ((argument & RAM) && (sscanf (str_argument + arg_offset, "%lu", &number) == 1))
    {
        argument |= IMMED_NUM;

        memcpy (assembler->code + assembler->count_cmd - 1, &argument, sizeof (argument));

        memcpy (assembler->code + assembler->count_cmd, &number, sizeof (number));
        assembler->count_cmd += sizeof (number);

        return DONE_ASM;
    }

    if ((argument & RAM) && (plus_position != NULL))
    {
        argument |= IMMED_NUM;
        argument |= REGISTER;

        strtok (str_argument, "+");
        sscanf (str_argument + arg_offset, "%s", reg);
        sscanf (plus_position + 1, "%lu", &number);

        memcpy (assembler->code + assembler->count_cmd - 1, &argument, sizeof (argument));

        number_reg = (size_t) register_num (reg);
        if (number_reg == (size_t) -1)
        {
            return INVAL_REG;
        }

        memcpy (assembler->code + assembler->count_cmd, &number_reg, sizeof (number_reg));
        assembler->count_cmd += sizeof (number_reg);

        memcpy (assembler->code + assembler->count_cmd, &number, sizeof (number));
        assembler->count_cmd += sizeof (number);

        return DONE_ASM;
    }

    if (sscanf (str_argument + arg_offset, "%s", reg) == 1)
    {
        argument |= REGISTER;

        memcpy (assembler->code + assembler->count_cmd - 1, &argument, sizeof (argument));

        number_reg = (size_t) register_num (reg);
        if (number_reg == (size_t) -1)
        {
            return INVAL_REG;
        }

        memcpy (assembler->code + assembler->count_cmd, &number_reg, sizeof (number_reg));
        assembler->count_cmd += sizeof (number_reg);

        return DONE_ASM;
    }

    return POP_INVAL_ARG;
}

static bool jumper (char* const cmd, assembler_t* const assembler)
{
    ASSERT (cmd != NULL, "Invalid pointer for cmd in jumper\n");
    ASSERT (assembler != NULL, "Invalid pointer for assembler in jumper\n");

    #define CASE_JUMP(jump_cmd)                                                                             \
        if (my_strcmp (cmd, #jump_cmd) == 0)                                                                \
        {                                                                                                   \
            assembler->code [assembler->count_cmd - 1] = (((command_t) jump_cmd) << ARGUMENT_TYPE);         \
            char label [LABEL_NAME_LEN] = "";                                                               \
            if (sscanf (assembler->input_buffer + assembler->input_offset, "%s", label) == EOF)             \
            {                                                                                               \
                return false;                                                                               \
            }                                                                                               \
            size_t number_of_label = label_num (label, assembler);                                          \
            memcpy (assembler->code + assembler->count_cmd, &number_of_label, sizeof (number_of_label));    \
            assembler->count_cmd += sizeof (number_of_label);                                               \
            assembler->input_offset += strlen (label);                                                      \
            assembler->input_offset = skip_space_symbols (assembler->input_buffer, assembler->input_offset);\
                                                                                                            \
            return true;                                                                                    \
        }

    LOG (DEBUG, "The function jumper got arguments:\n"
                "| cmd = %p | assembler = %p | input = %p |\n", cmd, assembler, assembler->input_buffer);

    CASE_JUMP (JMP);
    CASE_JUMP (JA);
    CASE_JUMP (JAE);
    CASE_JUMP (JB);
    CASE_JUMP (JBE);
    CASE_JUMP (JE);
    CASE_JUMP (JNE);
    CASE_JUMP (CALL);

    #undef CASE_JUMP

    return false;
}
