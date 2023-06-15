#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

#include "definitions.h"

#define ZERO_REGISTER_VALUE 0x1F

int count_lines(char *inputFile);

bool containsColon(char* line);

void deleteColon(char *line, unsigned long lineLength);

void freeLines(char **lines, int numLines);

char **tokenizer(char *line, int *numToken, char **tokens);

bool isRegister(const char* reg);

int registerToBinary(char* reg);

uint32_t getSF(const char* reg);

bool isStringInSet(char *target, char *set[], size_t setSize);

int getSubstringAsInt(char *string, int start, int size);

uint32_t findAddressOfLabel(char *label, symbol_table_row *symbol_table);

char *getZeroRegister(const char *reg);

#endif
