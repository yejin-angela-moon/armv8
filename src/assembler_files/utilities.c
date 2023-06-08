#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "definitions.h"

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

bool isStringInSet(const char *target, const char *set[], size_t setSize) {
  for (size_t i = 0; i < setSize; i++) {
    if (strcmp(target, set[i]) == 0) {
      return true; // found the string in the set
    }
  }
  return false; // the string was not found in the set
}


void freeLines(char **lines, int numLines){
    for (int i = 0; i < numLines; ){
        free(lines[i]);
    }
    free(lines);
}