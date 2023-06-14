#ifndef DEFINITION_H
#define DEFINITION_H

#include <stdint.h>
#include <string.h>

typedef struct {
    char *label;
    uint32_t address;
} row;

#define MAX_LINE_LENGTH 256
#define MAX_TOKEN 6
#define delimiter ", #"
#define REGISTER_OFFSET 0x81A
#define MASK_TO_19BITS 0x7FFFF
#define NOP_INSTRUCTION 0xd503201F
#define MASK_9 0x01FF
#define B_INSTRUCTION 0x14000000
#define MASK_TO_26BITS 0x3FFFFFF
#define BR_INSTRUCTION 0xD61F0000
#define COND_INSTRUCTION 0x54000000


#endif
