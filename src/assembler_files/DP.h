#ifndef DP_H
#define DP_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "utilities.h"

void DPImm(char* tokens[], int numTokens, char* result);
void DPReg(char* tokens[], int numTokens, char* result);
void DP(char* tokens[], int numTokens, char* result);

#include "definition.h"

#endif
