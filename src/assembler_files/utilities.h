#ifndef UTILITIES_H
#define UTILITIES_H

#include "definition.h"
#include "utilities.h"

int count_lines(char *inputFile);
bool containColon(char* line);
void freeLines(char **lines, int numLines);
char **tokenizer(char *line, int *numToken);
uint32_t findAddressTable(char *lable, row *table);
int getNum(char *string, int start, int size);
bool isStringInSet(const char *target, const char *set[], size_t setSize);
char* stringToBinary(char* string, int nbits);
bool isRegister(const char* reg);
char* registerToBinary(char* reg);
char* getSF(const char* reg);

#endif
