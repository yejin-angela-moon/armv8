#ifndef DEFINITION_H
#define DEFINITION_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#define NUM_REGISTERS 31
#define HALT_INSTRUCTION 0x8a000000
#define NOP_INSTRUCTION 0xD503201F
#define MEMORY_SIZE (2 * 1024 * 1024)  // 2 MiB
#define LOWER_32_BITS 0xFFFFFFFF
#define SIGN_EXTEND_32BITS 0xFFFFFFFF00000000
#define SIGN_EXTEND_19BITS 0xFFFFFFFFFFF80000
#define SIGN_EXTEND_9BITS 0xFFFFFE00
#define MASK_1_BYTE 0xFF

typedef struct {
    bool N;
    bool Z;
    bool C;
    bool V;
} Pstate;

typedef struct {
    Pstate pstate;
    uint64_t generalRegisters[NUM_REGISTERS];
    uint32_t currAddress; // address to represent PC
    uint32_t *memory;
} state;

#endif
