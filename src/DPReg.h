#ifndef UNTITLED_DPREG_H
#define UNTITLED_DPREG_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include "definitions.h"
#include "utilities.h"

void arithmeticDPReg(uint8_t opc, uint8_t opr, uint8_t rd, uint8_t rn, uint8_t rm, uint8_t operand, bool sf, state *state);

void logicalDPReg(uint8_t opc, uint8_t opr, uint8_t rd, uint8_t rn, uint8_t rm, uint8_t operand, bool sf, state *state);

void multiplyDPReg(uint8_t rd, uint8_t rn, uint8_t rm, uint8_t operand, bool sf, state *state);

void DPReg(uint32_t instruction, state *state);

#endif
