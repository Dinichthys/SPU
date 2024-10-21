#ifndef ASSEMBLER_H
#define ASSEMBLER_H

typedef struct label
{
    char name [100];

    size_t pointer;
} label_t;

typedef struct assembler
{
    label_t labels [100];

    size_t labels_size;

    size_t* code;

    size_t count_cmd;
} assembler_t;

long compile (assembler_t* const assembler, FILE* const input);

#endif // ASSEMBLER_H
