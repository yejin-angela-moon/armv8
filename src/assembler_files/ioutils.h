#ifndef IOUTILS_H
#define IOUTILS_H

#include <string.h>

char** readFile(int lineNum, int *countLabel ,char *filename);
row * makeSymbolTable(row *table, int lineNum, char **lines);

#include "definition.h"

#endif