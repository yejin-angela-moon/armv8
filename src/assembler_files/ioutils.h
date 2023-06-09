#ifndef IOUTILS_H
#define IOUTILS_H

#include "definition.h"

char** readFile(int lineNum, int *countLabel ,char *filename);
void makeSymbolTable(row *table, int lineNum, char **lines);

#endif