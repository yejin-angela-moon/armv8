#ifndef UNTITLED_DPIMM_H
#define UNTITLED_DPIMM_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include "definitions.h"
#include "utilities.h"

static void arithmetic_immediate(uint8_t sf, uint8_t opc, uint32_t operand, uint8_t Rd, state *state);

static void wide_move_immediate(uint8_t sf, uint8_t opc, uint32_t operand, uint8_t Rd, state *state);

void DPImm(uint32_t instruction, state *state);

#endif
