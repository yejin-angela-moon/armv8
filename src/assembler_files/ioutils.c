#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "definition.h"
#include "utilities.h"

char **readFile(int lineNum, int *countLabel, char *filename) {
    FILE *file = fopen(filename, "r");
    assert(file != NULL);

    char **strings = malloc(lineNum * sizeof(char *));
    assert(strings != NULL);

    for (int i = 0; i < lineNum; i++) {
        strings[i] = malloc(MAX_LINE_LENGTH * sizeof(char));
        assert(strings[i] != NULL);
    }

    bool thisLineEmpty = true;
    bool prevCharNewline = true;
    int stringIndex = 0;
    int indexInsideString = 0;
    int c;
    while ((c = fgetc(file)) != EOF) {
        if (c != '\n' && !isspace(c)) {
            thisLineEmpty = false;
            strings[stringIndex][indexInsideString] = (char) ch;
            indexInsideString++;
            prevCharNewline = false;
        } else if (c == '\n') {
            indexInsideString = 0;
            if (!prevCharNewline) {
                stringIndex++;
            }
            prevCharNewline = true;
        } else { // isspace(c)
            if (!thisLineEmpty) {
                strings[stringIndex][indexInsideString] = (char) ch;
                indexInsideString++;
            }
            prevCharNewline = false;
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
