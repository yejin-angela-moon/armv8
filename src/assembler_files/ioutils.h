#ifndef IOUTILS_H
#define IOUTILS_H

#include <string.h>

char** readFile(int lineNum, int *countLabel ,char *filename);

void makeSymbolTable(symbol_table_row *symbol_table, int lineNum, char **lines);

#include "definition.h"

#endif
