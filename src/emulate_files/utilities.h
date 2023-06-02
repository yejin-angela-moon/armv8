#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include "definitions.h"

void writeRegister (uint8_t registerIndex, uint64_t newValue, uint8_t sf, uint64_t *generalRegisters);

uint64_t readRegister (uint8_t registerIndex, uint8_t sf, uint64_t *generalRegisters);

uint32_t extractBits(uint64_t n, uint8_t startIndex, uint8_t endIndex);

uint64_t bitShift(uint8_t shift, uint64_t n, uint8_t operand);

void update_pstate(uint64_t result, uint64_t operand1, uint64_t operand2, bool is_subtraction, Pstate *pstate);

void inc_PC (state *state);

state *initialise(void);

#endif
