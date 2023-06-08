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

uint8_t registerToNumber(char reg[]) {
    // ex: "x12" -> 12
    assert(reg[0] == 'w' || reg[0] == 'x');
    return atoi(reg + 1);
}
