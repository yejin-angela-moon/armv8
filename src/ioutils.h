#ifndef IOUTILS_H
#define IOUTILS_H

#include "definition.h"
#include <stdio.h>
#include <stdint.h>

void readFile(uint32_t* memory, char* filename);
void printStateToFile( uint32_t* memory, char* filename, Pstate pstate, uint32_t currAddress);
void printToString( uint32_t* memory, Pstate pstate, uint32_t currAddress);

#endif
