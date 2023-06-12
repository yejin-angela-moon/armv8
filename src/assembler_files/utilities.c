#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "definition.h"

int count_lines(char *inputFile){
  FILE *fp = fopen(inputFile, "r");

  if (fp == NULL){
    printf("Failed to open the file.\n");
    exit(1);
  }

  int lineCount = 0;
  int character;

  while((character = fgetc(fp)) != EOF){
    if(character == "\n"){
      lineCount++;
    }
  }

  return lineCount;
}

bool containColon(char* line){
  return(strchr(line, ':') != NULL);
}

char **tokenizer(char *line, int *numToken){
  int i = 0;
  char *string;
  char **token = malloc(MAX_TOKEN * sizeof(char*));

  while (string != NULL){
    token[i] = string;
    i++;
    string = strtok(NULL, delimiter);
  }
  *numToken = i;
  return token;
}

void freeLines(char **lines, int numLines){
  for (int i = 0; i < numLines; i++){
    free(lines[i]);
  }
  free(lines);
}

void call_function(char* name, func_map* function_table, int numOfFuncs, char* tokens[], int numOfTokens) {
  for (int i = 0; i < numOfFuncs/sizeof(function_table[0]); i++) {
    if (strcmp(function_table[i].name, name) == 0) {
      function_table[i].func(tokens, numOfTokens);
      return;
    }
  }
  fprintf(stderr, "Function not found\n");
}

bool isStringInSet(const char *target, const char *set[], size_t setSize) {
  for (size_t i = 0; i < setSize; i++) {
    if (strcmp(target, set[i]) == 0) {
      return true; // found the string in the set
    }
  }
  return false; // the string was not found in the set
}

bool isRegister(const char* reg) {
  return (reg[0] == 'w' || reg[0] == 'x');
}

static char* decToBinary(uint32_t x, int nbits) {
  char* res = (char *) malloc(32 * sizeof(char));
  assert(res != NULL);
  uint32_t mask = 1 << (nbits - 1);
  for (int i = 0; i < nbits; i++) {
    if ((x & mask) == 0) {
      strcat(res, "0");
    } else {
      strcat(res, "1");
    }
    mask = mask >> 1;
  }
  strcat(res, "\0");
  return res;
}

static uint32_t stringToNumber(char* string) {
  return (uint32_t)strtol(string, NULL, 0);
}

char* stringToBinary(char* string, int nbits) {
  return decToBinary(stringToNumber(string), nbits);
}

char* registerToBinary(char* reg) {
  // ex: "x11" -> "1011"
  assert(isRegister(reg));
  if (strcmp(reg + 1, "zr") == 0) {
    return "11111";
  }
  return decToBinary(stringToNumber(reg + 1), 5);
}

char* getSF(const char* reg) {
  assert(isRegister(reg));
  return reg[0] == 'w' ? "0" : "1";
}

char* opcArithmetic(const char* opcode) {
  if (strcmp("add", opcode) == 0) {
    return "00";
  } else if (strcmp("adds", opcode) == 0) {
    return "01";
  } else if (strcmp("sub", opcode) == 0) {
    return "10";
  } else if (strcmp("subs", opcode) == 0) {
    return "11";
  } else {
    fprintf(stderr, "invalid opcode\n");
    exit(1);
  }
}

char* opcWideMove(const char* opcode) {
  if (strcmp("movk", opcode) == 0) {
    return "11";
  } else if (strcmp("movn", opcode) == 0) {
    return "00";
  } else if (strcmp("movz", opcode) == 0) {
    return "10";
  } else {
    fprintf(stderr, "invalid opcode\n");
    exit(1);
  }
}

char* getShiftCode(char* shift) {
  if (strcmp("lsl", shift) == 0) {
    return "00";
  } else if (strcmp("lsr", shift) == 0) {
    return "01";
  } else if (strcmp("asr", shift) == 0) {
    return "10";
  } else if (strcmp("ror", shift) == 0) {
    return "11";
  } else {
    fprintf(stderr, "invalid shift name\n");
    exit(1);
  }
}
