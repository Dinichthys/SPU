#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

#include <stddef.h>
#include <stdio.h>

#include "../program.h"

static const char COMMENT_SYMBOL  = ';';
static const int STR_ARGUMENT_LEN = 100;
static const int CMD_NAME_LEN     = 40;
static const int REG_NAME_LEN     = 5;
static const int LABEL_NAME_LEN   = 100;
static const int NUMBER_LABELS    = 100;

enum ASSEMBLER_ERROR
{
    DONE_ASM              = 0,
    PUSH_OR_POP_INVAL_ARG = 1,
    INVAL_REG             = 2,
    INVAL_COMMAND         = 3,
    CANT_CTOR_ASM         = 4,
    CANT_DTOR_ASM         = 5,
    CANT_WRITE_RESULT_ASM = 6,
    REDECLARATION_LABEL   = 7,
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

    const char* output;
} assembler_t;

enum ASSEMBLER_ERROR compile      (assembler_t* const assembler);
enum ASSEMBLER_ERROR asm_ctor     (assembler_t* const assembler, const char* name_input);
enum ASSEMBLER_ERROR asm_dtor     (assembler_t* const assembler);
enum ASSEMBLER_ERROR write_result (assembler_t* const assembler);

#endif // ASSEMBLER_H_
