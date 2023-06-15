#include <stdio.h>
#include <stdlib.h>

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

    int ch;
    bool isLineEmpty = true;
    int stringIndex = 0;
    int indexInsideString = 0;

    while ((ch = fgetc(file)) != EOF) {
        if (ch == ':') {
            (*countLabel)++;
        }

        if (ch == '\n') {
            if (!isLineEmpty) {
                strings[stringIndex][indexInsideString] = '\0';
                stringIndex++;
            }
            isLineEmpty = true;
            indexInsideString = 0;
        } else {
            isLineEmpty = false;
            strings[stringIndex][indexInsideString] = (char) ch;
            indexInsideString++;
        }
    }

    fclose(file);
    return strings;
}

void makeSymbolTable(symbol_table_row *symbol_table, int lineNum, char **lines) {
    int j = 0;
    int addr = 0;

    for (int i = 0; i < lineNum; i++) {
        if (containsColon(lines[i])) {
            symbol_table[j].address = addr * 4;

            deleteColon(lines[i], strlen(lines[i]));
            // deletes colon from every line with label

            symbol_table[j].label = lines[i];
            j++;
        } else if (strlen(lines[i]) > 2) {
            addr++;
        }
    }
}
