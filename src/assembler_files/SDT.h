#ifndef SDT_H
#define SDT_H

#include <stdint.h>

#include "definition.h"

#define SDT_INSTRUCTION 0xB8000000
#define LL_INSTRUCTION 0x18000000

uint32_t SDT(char **tokens, symbol_table_row *symbol_table, int countTokens, uint32_t currAddress);

#include "utilities.h"

#endif
