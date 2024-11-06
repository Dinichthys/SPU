#include "disassembler.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "../program.h"
#include "../Assert/my_assert.h"
#include "../Logger/logging.h"
#include "../My_stdio/my_stdio.h"

static enum DISASSEMBLER_ERROR tolower_write   (disassembler_t* const disassembler, const char* const string);
static enum DISASSEMBLER_ERROR push_or_pop_cmd (disassembler_t* const disassembler, const command_t argument);

#define CASE_CMD(command)                                                                   \
    case command:                                                                           \
    {                                                                                       \
        LOG (DEBUG, #command " was recognised\n");                                          \
        tolower_write (disassembler, #command);                                             \
        sprintf (disassembler->output_buffer + disassembler->output_offset, "\n");          \
        disassembler->output_offset += 1;                                                   \
        break;                                                                              \
    }

#define CASE_JUMP(jump_cmd)                                                                                 \
    case jump_cmd:                                                                                          \
    {                                                                                                       \
        LOG (DEBUG, #jump_cmd " was recognised\n");                                                         \
        size_t pointer = 0;                                                                                 \
        memcpy (&pointer, disassembler->code + disassembler->ip, sizeof (pointer));                         \
                                                                                                            \
        tolower_write (disassembler, #jump_cmd);                                                            \
        sprintf (disassembler->output_buffer + disassembler->output_offset, " %lu\n", pointer);             \
        char number_str [NUMBER_LEN] = "";                                                                  \
        sprintf (number_str, "%lu", pointer);                                                               \
        disassembler->output_offset += strlen (number_str) + 2;                                             \
        disassembler->ip += sizeof (pointer);                                                               \
                                                                                                            \
        break;                                                                                              \
    }
// ----------------------------------------------------------------------------------------------

enum DISASSEMBLER_ERROR disassembling (disassembler_t* const disassembler)
{
    ASSERT (disassembler != NULL, "Invalid argument for disassembling = %p\n", disassembler);

    while (disassembler->ip < disassembler->count_cmd)
    {

        command_t cmd_and_arg = disassembler->code [disassembler->ip];
        command_t cmd = (cmd_and_arg >> ARGUMENT_TYPE);
        command_t arg = (cmd_and_arg & ALL_ARGS);

        LOG (DEBUG, "Iteration №%lu\n"
                    "cmd_and_arg = %8.8b", disassembler->ip, cmd_and_arg);

        (disassembler->ip)++;

        switch (cmd)
        {
            case PUSH:
            {
                LOG (DEBUG, "PUSH was recognised\n");

                sprintf (disassembler->output_buffer + disassembler->output_offset, "push ");
                disassembler->output_offset += 5;

                enum DISASSEMBLER_ERROR result = push_or_pop_cmd (disassembler, arg);
                if (result != DONE_DISASM)
                {
                    return result;
                }
                break;
            }
            case POP:
            {
                LOG (DEBUG, "POP was recognised\n");

                sprintf (disassembler->output_buffer + disassembler->output_offset, "pop ");
                disassembler->output_offset += 4;

                enum DISASSEMBLER_ERROR result = push_or_pop_cmd (disassembler, arg);
                if (result != DONE_DISASM)
                {
                    return result;
                }
                break;
            }

            CASE_CMD (ADD);
            CASE_CMD (SUB);
            CASE_CMD (MUL);
            CASE_CMD (DIV);
            CASE_CMD (SQRT);
            CASE_CMD (IN);
            CASE_CMD (OUT);
            CASE_CMD (DUMP);

            CASE_JUMP (CALL);

            CASE_CMD (RET);

            CASE_CMD (DRAW);

            CASE_JUMP (JMP);
            CASE_JUMP (JA);
            CASE_JUMP (JAE);
            CASE_JUMP (JB);
            CASE_JUMP (JBE);
            CASE_JUMP (JE);
            CASE_JUMP (JNE);

            CASE_CMD (HLT);

            default:
            {
                return INVAL_COMMAND;
            }
        }
    }

    return DONE_DISASM;
}

#undef CASE_JUMP
#undef CASE_CMD

enum DISASSEMBLER_ERROR disasm_ctor (disassembler_t* const disassembler, const char* const name_file_input)
{
    ASSERT (disassembler    != NULL, "Invalid argument for disasm_ctor disassembler = %p\n", disassembler);
    ASSERT (name_file_input != NULL, "Invalid argument for disasm_ctor name_file_input = %p\n", name_file_input);

    FILE* input = NULL;
    FOPEN (input, name_file_input, "r+b");
    if (input == NULL)
    {
        return CANT_CTOR_DISASM;
    }

    fread (&(disassembler->count_cmd), 1, sizeof (disassembler->count_cmd), input);

    disassembler->ip = 0;

    disassembler->code = (command_t*) calloc (disassembler->count_cmd, sizeof (command_t));
    if (disassembler->code == NULL)
    {
        FCLOSE (input);
        return CANT_CTOR_DISASM;
    }

    fread (disassembler->code, sizeof (command_t), disassembler->count_cmd, input);
    FCLOSE (input);

    disassembler->output_buffer = (char*) calloc (disassembler->count_cmd * 10, sizeof (char));

    disassembler->output_offset = 0;

    return DONE_DISASM;
}

enum DISASSEMBLER_ERROR disasm_dtor (disassembler_t* const disassembler)
{
    ASSERT (disassembler != NULL, "Invalid argument for disasm_dtor disassembler = %p\n", disassembler);

    disassembler->count_cmd = 0;

    disassembler->ip = 0;

    free (disassembler->code);

    free (disassembler->output_buffer);

    disassembler->output_offset = 0;

    return DONE_DISASM;
}

enum DISASSEMBLER_ERROR write_result (disassembler_t* const disassembler, const char* const name_file_output)
{
    ASSERT (disassembler     != NULL, "Invalid argument for write_result: disassembler = %p\n", disassembler);
    ASSERT (name_file_output != NULL, "Invalid argument for write_result: name_file_output = %p\n", name_file_output);

    FILE* output_file = NULL;

    FOPEN (output_file, name_file_output, "w");
    if (output_file == NULL)
    {
        return CANT_WRITE_RESULT_DISASM;
    }

    fwrite (disassembler->output_buffer, sizeof (char), disassembler->output_offset, output_file);

    FCLOSE (output_file);

    return DONE_DISASM;
}

static enum DISASSEMBLER_ERROR tolower_write (disassembler_t* const disassembler, const char* const string)
{
    ASSERT (disassembler != NULL, "Invalid argument for tolower_write: disassembler = %p\n", disassembler);
    ASSERT (string       != NULL, "Invalid argument for tolower_write: string = %p\n", string);

    for (size_t i = 0; i < strlen (string); i++)
    {
        disassembler->output_buffer [disassembler->output_offset + i] = tolower (string [i]);
    }

    disassembler->output_offset += strlen (string);

    return DONE_DISASM;
}

static enum DISASSEMBLER_ERROR push_or_pop_cmd (disassembler_t* const disassembler, const command_t argument)
{
    ASSERT (disassembler != NULL, "Invalid argument for push_cmd: disassembler = %p\n", disassembler);

    if (argument & RAM)
    {
        sprintf (disassembler->output_buffer + disassembler->output_offset, "[");
        disassembler->output_offset++;
    }

    if (argument & REGISTER)
    {
        size_t number_reg = 0;

        memcpy (&number_reg, disassembler->code + disassembler->ip, sizeof (number_reg));

        if (number_reg >= COUNT_REGS)
        {
            return PUSH_INVAL_REG;
        }

        sprintf (disassembler->output_buffer + disassembler->output_offset, "%cx", 'a' + number_reg);

        disassembler->output_offset += 2;
        disassembler->ip += sizeof (number_reg);
    }

    if ((argument & REGISTER) && (argument & IMMED_NUM))
    {
        sprintf (disassembler->output_buffer + disassembler->output_offset, "+");
        disassembler->output_offset++;
    }

    if (argument & IMMED_NUM)
    {
        double temp = 0;

        memcpy (&temp, disassembler->code + disassembler->ip, sizeof (temp));

        sprintf (disassembler->output_buffer + disassembler->output_offset, "%.5lf", temp);

        char number_str [NUMBER_LEN] = "";
        sprintf (number_str, "%lf", temp);

        disassembler->output_offset += strlen (number_str) - 1;
        disassembler->ip += sizeof (temp);
    }

    if (argument & RAM)
    {
        sprintf (disassembler->output_buffer + disassembler->output_offset, "]");
        disassembler->output_offset++;
    }

    sprintf (disassembler->output_buffer + disassembler->output_offset, "\n");
    disassembler->output_offset++;

    return DONE_DISASM;
}

