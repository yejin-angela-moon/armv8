#ifndef DP_H
#define DP_H

#include "utilities.h"
#include "definition.h"

char* DPImm(char* tokens[], int numTokens);
char* DPReg(char* tokens[], int numTokens);
char* DP(char* tokens[], int numTokens);

#endif
