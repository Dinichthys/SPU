#ifndef SPU_H_
#define SPU_H_

#include <stdlib.h>
#include <stddef.h>

#include "../Stack/stack.h"
#include "../program.h"

typedef double spu_elem_t;

static const int    SLEEP_IN_RAM             = 0;
static const size_t RAM_SIZE                 = 10000;
static const size_t LEN_BORDER               = 10;
static const size_t SIZE_PIXEL               = 5;
static const size_t SIZE_STRINGS             = 20;
static const size_t SIZE_COLUMNS             = 20;
static const int    BIG_ENDIAN_SHIFT_ELEMENT = 7;

enum SPU_ERROR
{
    DONE_SPU                = 0,
    PUSH_INVAL_REG          = 1,
    CANT_PUSH_IN_PUSH_SPU   = 2,
    CANT_PUSH_IN_IN_SPU     = 3,
    POP_INVAL_REG           = 4,
    CANT_POP_IN_POP_SPU     = 5,
    CANT_POP_IN_OUT_SPU     = 6,
    INVAL_COMMAND           = 7,
    CANT_ADD_SPU            = 8,
    CANT_SUB_SPU            = 9,
    CANT_MUL_SPU            = 10,
    CANT_DIV_SPU            = 11,
    CANT_DUMP_SPU           = 12,
    CANT_JMP_SPU            = 13,
    CANT_JA_SPU             = 14,
    CANT_JAE_SPU            = 15,
    CANT_JB_SPU             = 16,
    CANT_JBE_SPU            = 17,
    CANT_JE_SPU             = 18,
    CANT_JNE_SPU            = 19,
    CANT_CTOR_SPU           = 20,
    CANT_DTOR_SPU           = 21,
    CANT_PUSH_CALL_SPU      = 22,
    CANT_POP_CALL_SPU       = 23,
    CANT_PUSH_IN_SQRT_SPU   = 24,
    CANT_POP_IN_SQRT_SPU    = 25,
    CANT_FIND_SQRT_SPU      = 26,
    CANT_POP_IN_SIN_SPU     = 27,
    CANT_PUSH_IN_SIN_SPU    = 28,
    CANT_POP_IN_COS_SPU     = 29,
    CANT_PUSH_IN_COS_SPU    = 30,
    CANT_POP_IN_MEOW_SPU    = 31,
    WITHOUT_HLT             = 32,
};

typedef struct spu
{
    size_t count_cmd;

    size_t ip;

    command_t* code;

    double regs [COUNT_REGS];

    stack_t stk;

    stack_t stack_func_call_ip;

    spu_elem_t* ram;
} spu_t;

enum SPU_ERROR processing (spu_t* const processor);
enum SPU_ERROR spu_ctor (spu_t* const processor, const char* const name_file_input);
enum SPU_ERROR spu_dtor (spu_t* const processor);

#endif // SPU_H_
