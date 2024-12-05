#ifndef PROGRAM_H_
#define PROGRAM_H_

typedef unsigned char command_t;

static const int ARGUMENT_TYPE = 3;
static const int COUNT_REGS    = 10;
__attribute__((unused)) static const char* REGISTERS [COUNT_REGS] = {"0x", "bx", "cx", "dx", "ex",
                                                                     "fx", "gx", "hx", "ix", "jx"};

enum COMMANDS
{
    PUSH  = 1,
    POP   = 2,
    ADD   = 3,
    SUB   = 4,
    MUL   = 5,
    DIV   = 6,
    SQRT  = 7,
    SIN   = 8,
    COS   = 9,
    IN    = 10,
    OUT   = 11,
    DUMP  = 12,
    JMP   = 13,
    JA    = 14,
    JAE   = 15,
    JB    = 16,
    JBE   = 17,
    JE    = 18,
    JNE   = 19,
    CALL  = 20,
    RET   = 21,
    DRAW  = 22,
    MEOW  = 23,
    BARK  = 24,
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
