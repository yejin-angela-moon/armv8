#ifndef IOUTILS_H
#define IOUTILS_H

#include "definitions.h"
#include <stdio.h>
#include <stdint.h>

void readFile(state* state, char* filename);
void printStateToFile(state* state, char* filename);
void printToString(state* state);

#endif
