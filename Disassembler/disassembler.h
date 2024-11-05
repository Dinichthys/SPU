#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include <stdlib.h>

#include "../program.h"

static const int NUMBER_LEN = 100;

enum DISASSEMBLER_ERROR
{
    DONE_DISASM              = 0,
    WITHOUT_HLT              = 1,
    INVAL_COMMAND            = 2,
    CANT_CTOR_DISASM         = 3,
    PUSH_INVAL_REG           = 4,
    POP_INVAL_REG            = 5,
    CANT_WRITE_RESULT_DISASM = 6,
};

typedef struct disassembler
{
    size_t count_cmd;

    size_t ip;

    command_t* code;

    char* output_buffer;

    size_t output_offset;
} disassembler_t;

enum DISASSEMBLER_ERROR disassembling (disassembler_t* const disassembler);
enum DISASSEMBLER_ERROR disasm_ctor (disassembler_t* const disassembler, const char* const name_file_input);
enum DISASSEMBLER_ERROR disasm_dtor (disassembler_t* const disassembler);
enum DISASSEMBLER_ERROR write_result (disassembler_t* const disassembler, const char* const name_file_output);

#endif // DISASSEMBLER_H
