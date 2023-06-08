#ifndef SDT_H
#define SDT_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include "definitions.h"
#include "utilities.h"

void SDT(uint32_t instruction, state *state);

void LL(uint32_t instruction, state *state);

#endif
