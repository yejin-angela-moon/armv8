#ifndef DEFINITION_H
#define DEFINITION_H

#include <stdint.h>

#define MAX_LINE_LENGTH 256
#define MAX_TOKEN 6
#define delimiter ", #"
#define LL_TOKEN_NUM 3
#define REGISTER_OFFSET 0x81A
#define LL_INSTRUC 0x18000000
#define MASK_TO_19BITS 0x7FFFF
#define SDT_INSTRUC 0xB8000000
#define NOP_INSTRUCTION 0xd503201F

typedef struct {
    char *label;
    uint32_t address;
} row;

#endif
