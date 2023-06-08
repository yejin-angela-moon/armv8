#ifndef IOUTILS_H
#define IOUTILS_H

#include "definition.h"
#include <stdio.h>
#include <stdint.h>

char** readFile(int lineNum, int *countLabel ,char *filename);
void makeSymbolTable(row *table, int lineNum, char **lines);

#endif
