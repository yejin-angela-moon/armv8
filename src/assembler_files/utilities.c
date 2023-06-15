#include "utilities.h"

int count_lines(char *inputFile) {
    FILE *file;
    int ch;
    int linesCount = 0;
    bool isLineEmpty = true;

    file = fopen(inputFile, "r");
    assert(file != NULL);

    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            if (!isLineEmpty) {
                linesCount++;
            }
            isLineEmpty = true;
        } else {
            isLineEmpty = false;
        }
    }

    fclose(file);
    return linesCount;
}

bool containsColon(char *line) {
    return (strchr(line, ':') != NULL);
}

void deleteColon(char *line, unsigned long lineLength) {
    while (isspace(line[lineLength - 1]) || line[lineLength - 1] == ':') {
        line[lineLength - 1] = '\0';
        lineLength--;
    }
}

char **tokenizer(char *line, int *numToken, char **tokens) {
    // splits string up into tokens
    int i = 0;
    tokens[0] = strtok(line, delimiter);
    while (tokens[i] != NULL) {
        i++;
        tokens[i] = strtok(NULL, delimiter);
    }
    *numToken = i;
    return tokens;
}

void freeLines(char **lines, int numLines) {
    for (int i = 0; i < numLines; i++) {
        free(lines[i]);
    }
    free(lines);
}

bool isStringInSet(char *target, char *set[], size_t setSize) {
    for (size_t i = 0; i < setSize; i++) {
        if (strcmp(target, set[i]) == 0) {
            return true;
        }
    }
    return false;
}

bool isRegister(const char *reg) {
    return (tolower(reg[0]) == 'w' || tolower(reg[0]) == 'x');
}

int registerToBinary(char *reg) {
    // ex: "x11" -> 0b1011
    assert(isRegister(reg));
    if (strcmp(reg + 1, "zr") == 0) {
        return 0x1F;
    }
    return (int) strtol((reg + 1), NULL, 0);
}

char *getSF(const char *reg) {
    assert(isRegister(reg));
    return tolower(reg[0] == 'w') ? "0" : "1";
}

int getSubstringAsInt(char *string, int start, int size) {
    char substring[size];
    strncpy(substring, string + start, size);
    return (int) strtol(substring, NULL, 0);
}

uint32_t findAddressOfLabel(char *label, symbol_table_row *symbol_table) {
    int i = 0;
    while (symbol_table[i].label[0] != '\0') {
        if (strcmp(symbol_table[i].label, label) == 0) {
            return symbol_table[i].address;
        }
        i++;
    }
    return i;
}

char *getZeroRegister(const char *reg) {
    return tolower(reg[0]) == 'w' ? "wzr" : "xzr";
}
