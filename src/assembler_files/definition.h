#ifndef DEFINITION_H
#define DEFINITION_H

#include <stddef.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>

const char *dpSet[] = {"add", "adds", "sub", "subs", "movn", "movk", "movz",
                       "madd", "msub", "and", "bic", "orr", "orn", "eon",
                       "eor", "ands", "bics", "tst", "neg", "mvn", "mov",
                       "mul"};

typedef struct {
    char *label;
    uint32_t address;
} row;

#define MAX_LINE_LENGTH 256
#define MAX_TOKEN 6
#define delimiter ", #"
#define NOP_INSTRUCTION 0xd503201F
#define dpSetSize 22

#endif
