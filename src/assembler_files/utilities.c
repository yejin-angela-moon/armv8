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

int getNum(char *string, int start, int size){
    char substring[size];
    strncpy(substring, string + start, size);
    substring[size - 1];

    return atoi(substring);
}

int getNum(char *string, int start, int size){
    char substring[size];
    strncpy(substring, string + start, size);
    substring[size - 1];

    return atoi(substring);
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

bool isStringInSet(const char *target, const char *set[], size_t setSize) {
  for (size_t i = 0; i < setSize; i++) {
    if (strcmp(target, set[i]) == 0) {
      return true; // found the string in the set
    }
  }
  return false; // the string was not found in the set
}

uint32_t findAddressTable(char *lable, row *table){
    int i = 0;
    while(table[i].label[0] == '\0'){
        if (strcmp(table[i].label, lable) == 0){
            return table[i].address;
        }
        i++;
    }
}


uint8_t registerToNumber(char reg[]) {
  // ex: "x12" -> 12
  assert(reg[0] == 'w' || reg[0] == 'x');
  return atoi(reg + 1);
}
