#ifndef DEFINITION_H
#define DEFINITION_H

#include <stddef.h>
#include <stdbool.h>

#define NUM_REGISTERS 31
#define HALT_INSTRUCTION 0x8a000000
#define MEMORY_SIZE (2 * 1024 * 1024)  // 2 MiB

typedef struct {
    bool N;
    bool Z;
    bool C;
    bool V;
} Pstate;

#endif // DEFINITION_H
