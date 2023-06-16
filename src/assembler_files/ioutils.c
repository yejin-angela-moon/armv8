#include <stdio.h>
#include <stdlib.h>

#include "utilities.h"

char **readFile(int lineNum, int *countLabel, char *filename) {
  FILE *file = fopen(filename, "r");
  assert(file != NULL);

  char **strings = malloc(lineNum * sizeof(char *));
  assert(strings != NULL);
  for (int i = 0; i < lineNum; i++) {
    strings[i] = malloc(MAX_LINE_LENGTH * sizeof(char)); // malloc for each string
    assert(strings[i] != NULL);
  }

  int ch;
  bool isLineEmpty = true;
  int stringIndex = 0; // count lines
  int indexInsideString = 0; // count characters within a line

  while ((ch = fgetc(file)) != EOF) {
    if (ch == ':') { // indicate colon after label name
      (*countLabel)++; // count the number of labels
    }

    if (ch == '\n') {
      if (!isLineEmpty) {
        strings[stringIndex][indexInsideString] = '\0'; // if line is not empty
        stringIndex++;
      }
      isLineEmpty = true; // initialised to true for new line
      indexInsideString = 0;
    } else {
      isLineEmpty = false; // false if there is non-space character in the line
      strings[stringIndex][indexInsideString] = (char) ch; // write the character to the string
      indexInsideString++; // count character within line
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
