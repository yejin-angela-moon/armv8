#ifndef SDT_H
#define SDT_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "definition.h"
#include "utilities.h"

uint32_t SDT(char **token, row *table, int countToken, uint32_t currAddress);

#endif
