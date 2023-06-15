#ifndef B_H
#define B_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "definitions.h"

#define CONDITIONAL_INSTRUCTION 0x54000000
#define B_INCREMENT 0x14000000
#define BR_INCREMENT 0xD61F0000
#define MASK_19_BIT 0x7FFFF
#define MASK_26_BIT 0x3FFFFFF

uint32_t B(symbol_table_row *symbol_table, char **tokens, uint32_t currAddress);

#endif
