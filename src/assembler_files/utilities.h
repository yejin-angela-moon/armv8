#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "definition.h"

int count_lines(char *inputFile);
bool containColon(char* line);
void freeLines(char **lines, int numLines);
char **tokenizer(char *line, int *numToken, char ** token);
char* decToBinary(uint32_t x, int nbits);
uint32_t stringToNumber(char* string);
char* stringToBinary(char* string, int nbits);
bool isRegister(const char* reg);
char* registerToBinary(char* reg);
char* getSF(const char* reg);
bool isStringInSet(char *target, char *set[], size_t setSize);
int getNum(char *string, int start, int size);
uint32_t findAddressTable(char *label, row *table);

#endif
