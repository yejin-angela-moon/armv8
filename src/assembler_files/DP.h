#ifndef DP_H
#define DP_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "utilities.h"

char* DPImm(char* tokens[], int numTokens, char *res);
char* DPReg(char* tokens[], int numTokens, char *res);
char* DP(char* tokens[], int numTokens, char *res);

#include "definition.h"

#endif
