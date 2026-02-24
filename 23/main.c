#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

enum InstructionKind {
    HLF,
    TPL,
    INC,
    JMP,
    JIE,
    JIO,
};

struct Instruction {
    enum InstructionKind kind;
    char reg;
    int offset;
};

struct Program {
    struct Instruction *array;
    size_t capacity;
    size_t count;
};

void program_reserve(struct Program *program, size_t minimum) {
    if (minimum <= program->capacity) return;

    if (program->capacity == 0) {
        program->capacity = 8;
    }

    while (program->capacity < minimum) {
        program->capacity *= 2;
    }

    program->array = realloc(program->array, sizeof(struct Instruction) * program->capacity);
}

void program_push(struct Program *program, struct Instruction instruction) {
    program_reserve(program, program->count + 1);
    program->array[program->count++] = instruction;
}

int register_b(bool part2, const struct Program *program) {
    unsigned int reg[2] = {0};
    int pc = 0;

    if (part2) {
        reg[0] = 1;
    }

    while (pc < program->count) {
        struct Instruction ins = program->array[pc];

        switch (ins.kind) {
            case HLF: reg[ins.reg] /= 2; pc++; break;
            case TPL: reg[ins.reg] *= 3; pc++; break;
            case INC: reg[ins.reg] += 1; pc++; break;
            case JMP: pc += ins.offset; break;
            case JIE: if (reg[ins.reg] % 2 == 0) pc += ins.offset; else pc++; break;
            case JIO: if (reg[ins.reg] == 1) pc += ins.offset; else pc++; break;
        }
    }

    return reg[1];
}

int main(void) {
    struct Program program;
    char buffer[256];

    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if (buffer[0] == '\n') break;

        enum InstructionKind kind;
        char r;
        int offset;

        if (sscanf(buffer, "hlf %c", &r) == 1) {
            kind = HLF;
        } else if (sscanf(buffer, "tpl %c", &r) == 1) {
            kind = TPL;
        } else if (sscanf(buffer, "inc %c", &r) == 1) {
            kind = INC;
        } else if (sscanf(buffer, "jmp %d", &offset) == 1) {
            kind = JMP;
        } else if (sscanf(buffer, "jie %c, %d", &r, &offset) == 2) {
            kind = JIE;
        } else if (sscanf(buffer, "jio %c, %d", &r, &offset) == 2) {
            kind = JIO;
        } else {
            fprintf(stderr, "ERROR: failed to parse \"%s\"\n", buffer);
            return 1;
        }

        struct Instruction instruction = {kind, r - 'a', offset};
        program_push(&program, instruction);
    }

    printf("Part 1: %u\n", register_b(false, &program));
    printf("Part 2: %u\n", register_b(true, &program));

    return 0;
}
