#include <stdio.h>
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


void freeLines(char **lines, int numLines){
    for (int i = 0; i < numLines; ){
        free(lines[i]);
    }
    free(lines);
}