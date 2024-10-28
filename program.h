#ifndef PROGRAM_H_
#define PROGRAM_H_

typedef unsigned char command_t;

static const int ARGUMENT_TYPE = 3;
static const int COUNT_REGS    = 10;

enum COMMANDS
{
    PUSH  = 1,
    POP   = 2,
    ADD   = 3,
    SUB   = 4,
    MUL   = 5,
    DIV   = 6,
    SQRT  = 7,
    IN    = 8,
    OUT   = 9,
    DUMP  = 10,
    JMP   = 11,
    JA    = 12,
    JAE   = 13,
    JB    = 14,
    JBE   = 15,
    JE    = 16,
    JNE   = 17,
    CALL  = 18,
    RET   = 19,
    DRAW  = 20,
    HLT   = 0,
};

enum TYPE_OF_ARGUMENTS
{
    IMMED_NUM   = 1,
    REGISTER    = 1 << 1,
    RAM         = 1 << 2,
    ALL_ARGS    = IMMED_NUM + REGISTER + RAM,
};

#endif // PROGRAM_H_
