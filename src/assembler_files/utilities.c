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