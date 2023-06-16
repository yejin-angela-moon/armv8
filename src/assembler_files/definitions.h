#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <stdint.h>
#include <string.h>

typedef struct {
    char *label;
    uint32_t address;
} symbol_table_row;

#define MAX_LINE_LENGTH 256
#define MAX_TOKEN 7
#define delimiter ", #"
#define REGISTER_OFFSET 0x81A
#define MASK_TO_19BITS 0x7FFFF
#define NOP_INSTRUCTION 0xd503201F
#define ZERO_REGISTER_VALUE 31

#endif
