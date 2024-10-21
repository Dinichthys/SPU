#ifndef SPU_H
#define SPU_H

#include "Stack/stack.h"
#include "program.h"

typedef struct spu
{
    size_t count_cmd = 0;

    size_t ip = 0;

    size_t* code = NULL;

    size_t regs [10] = {0};

    stack_t stk;
} spu_t;

#endif // SPU_H
