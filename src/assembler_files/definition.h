#ifndef DEFINITION_H
#define DEFINITION_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#define MAX_LINE_LENGTH 256
#define MAX_TOKEN 6
#define delimiter ", #"
#define LL_TOKEN_NUM 3
#define REGISTER_OFFSET 0x81A
#define LL_INSTRUC 0x18000000
#define MASK_TO_19BITS 0x7FFFF
#define SDT_INSTRUC 0xB8000000
#define NOP_INSTRUCTION 0xd503201F

const char *dpSet[] = {"add", "adds", "sub", "subs", "movn", "movz", "movk", "and", "bic", "orr", "orn", "eon", "eor", "ands", "bics", "madd", "msub"};
#define dpSetSize 17
const char *sdtSet[] = {"ldr", "str"};
#define sdtSetSize 2

typedef struct {
    char *label;
    uint32_t address;
} row;

#endif
