#ifndef SDT_H
#define SDT_H

#include <stdint.h>

#include "definition.h"

#define SDT_INSTRUC 0xB8000000
#define LL_INSTRUC 0x18000000
#define LL_TOKEN_NUM 3

uint32_t SDT(char **token, row *table, int countToken, uint32_t currAddress);

#include "utilities.h"

#endif
