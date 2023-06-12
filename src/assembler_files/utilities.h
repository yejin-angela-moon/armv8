#ifndef UTILITIES_H
#define UTILITIES_H

#include "definition.h"

int count_lines(char *inputFile);
bool containColon(char* line);
void freeLines(char **lines, int numLines);
char **tokenizer(char *line, int *numToken);
char* decToBinary(uint32_t x, int nbits);
uint32_t stringToNumber(char* string);
char* stringToBinary(char* string, int nbits);
bool isRegister(const char* reg);
char* registerToBinary(char* reg);
char* getSF(const char* reg);
bool isStringInSet(const char *target, const char *set[], size_t setSize);
int getNum(char *string, int start, int size);
uint32_t findAddressTable(char *label, row *table);

#endif
