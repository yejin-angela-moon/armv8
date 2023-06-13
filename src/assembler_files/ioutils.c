#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "utilities.h"

char **readFile(int lineNum, int *countLabel, char *filename) {
    FILE *file = fopen(filename, "r");
    assert(file != NULL);

    char **strings = malloc(lineNum * sizeof(char *));

    for (int i = 0; i < lineNum; i++) {
        strings[i] = malloc(MAX_LINE_LENGTH * sizeof(char));
    }

    int ch;
    bool isLineEmpty = 1;
    int stringIndex = 0;
    int indexInsideString = 0;
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            if (!isLineEmpty) {
                strings[stringIndex][indexInsideString] = '\0';
                stringIndex++;
            }
            isLineEmpty = 1;
            indexInsideString = 0;
        } else {
            if (ch == ':') {
                (*countLabel)++;
            }
            isLineEmpty = 0;
            strings[stringIndex][indexInsideString] = (char) ch;
            indexInsideString++;
        }
    }

    fclose(file);
    return strings;
}

void makeSymbolTable(row *table, int lineNum, char **lines){
  for (int i = 0; i < lineNum; i++){
    if (containColon(lines[i])){
      table[i].address = i * 4;
      table[i].label = lines[i];
      i--;
    }
  }
}
