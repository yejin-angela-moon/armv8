#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdbool.h>
#include <stdint.h>
#include "definition.h"

int count_lines(char *inputFile);
bool containColon(char* line);
void freeLines(char **lines, int numLines);
char **tokenizer(char *line, int *numToken);
uint32_t findAddressTable(char *lable, row *table);
int getNum(char *string, int start, int size);

#endif

