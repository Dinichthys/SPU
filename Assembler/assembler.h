#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

#include <stddef.h>
#include <stdio.h>

#include "../program.h"

static const int STR_ARGUMENT_LEN = 40;
static const int CMD_NAME_LEN     = 40;
static const int REG_NAME_LEN     = 5;
static const int LABEL_NAME_LEN   = 100;
static const int NUMBER_LABELS    = 100;
static const char* REGISTERS [COUNT_REGS] = {"ax", "bx", "cx", "dx", "ex", "fx", "gx", "hx", "ix", "jx"};

// NOTE делай функции + квадратка на асме

enum ASSEMBLER_ERROR
{
    DONE_ASM      = 0,
    PUSH_NO_ARG   = 1,
    POP_INVAL_ARG = 2,
    INVAL_REG     = 3,
    INVAL_COMMAND = 4,
    CANT_CTOR_ASM = 5,
    CANT_DTOR_ASM = 6,
};

typedef struct label
{
    char name [LABEL_NAME_LEN];

    size_t pointer;
} label_t;

typedef struct assembler
{
    label_t labels [NUMBER_LABELS];

    size_t labels_size;

    command_t* code;

    size_t count_cmd;

    char* input_buffer;

    size_t input_offset;

    FILE* output; // FIXME лучше таскать const char* чтоб в конце открыть и сразу записать
} assembler_t;

enum ASSEMBLER_ERROR compile (assembler_t* const assembler);
enum ASSEMBLER_ERROR asm_ctor (assembler_t* const assembler, const char* argv[]);
enum ASSEMBLER_ERROR asm_dtor (assembler_t* const assembler);
void write_result (assembler_t* const assembler);

#endif // ASSEMBLER_H_