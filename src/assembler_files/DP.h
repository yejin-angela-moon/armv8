#ifndef DP_H
#define DP_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "utilities.h"

uint32_t DPImm(char** tokens, int numTokens);
uint32_t DPReg(char** tokens, int numTokens);
uint32_t DP(char** tokens, int numTokens);

#include "definition.h"

#endif
