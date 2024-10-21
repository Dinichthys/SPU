#ifndef PROGRAM_H
#define PROGRAM_H

enum COMMANDS
{
    PUSH  = 1,
    PUSHR = 2,
    POP   = 3,
    ADD   = 4,
    SUB   = 5,
    MUL   = 6,
    DIV   = 7,
    OUT   = 8,
    DUMP  = 9,
    JMP   = 10,
    JA    = 11,
    JAE   = 12,
    JB    = 13,
    JBE   = 14,
    JE    = 15,
    JNE   = 16,
    HLT   = -1,
};

#endif // PROGRAM_H
