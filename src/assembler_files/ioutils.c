#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

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
//  bool prevCharNewline = true;
  int stringIndex = 0;
  int indexInsideString = 0;
  int c;
  while ((c = fgetc(file)) != EOF) {
    if (c != '\n' && !isspace(c)) {
      thisLineEmpty = false;
      strings[stringIndex][indexInsideString] = (char) c;
      indexInsideString++;
//      prevCharNewline = false;
    } else if (c == '\n') {
      indexInsideString = 0;
      if (!thisLineEmpty) {
        stringIndex++;
      }
      thisLineEmpty = true;
//      prevCharNewline = true;
    } else { // isspace(c)
      if (!thisLineEmpty) {
        strings[stringIndex][indexInsideString] = (char) c;
        indexInsideString++;
      }
//      prevCharNewline = false;
    }
  }

  fclose(file);
  return strings;
}


//char **readFile(int lineNum, int *countLabel, char *filename) {
//  FILE *file = fopen(filename, "r");
//  assert(file != NULL);
//
//  char **strings = malloc(lineNum * sizeof(char *));
//  assert(strings != NULL);
//
//  for (int i = 0; i < lineNum; i++) {
//    strings[i] = malloc(MAX_LINE_LENGTH * sizeof(char));
//    assert(strings[i] != NULL);
//    char line[MAX_LINE_LENGTH];
//    scanf(" %s", line);
//    strings[i] = line;
//    for (int j = 0; j < strlen(line); j++) {
//      if (strings[i][j] == ':') (*countLabel)++;
//      if (strings[i][j] == '\n') (strings[i][j] = '\0');
//    }
//  }
//  fclose(file);
//  return strings;
//}

//  int ch;
//  bool isLineEmpty = 1;
//  int stringIndex = 0;
//  int indexInsideString = 0;
//  while ((ch = fgetc(file)) != EOF) {
//    if (ch == '\n') {
//      if (!isLineEmpty) {
//        strings[stringIndex][indexInsideString] = '\0';
//        stringIndex++;
//      }
//      isLineEmpty = 1;
//      indexInsideString = 0;
//    } else {
//      if (ch == ':') {
//        (*countLabel)++;
//      }
//      isLineEmpty = 0;
//      strings[stringIndex][indexInsideString] = (char) ch;
//      indexInsideString++;
//    }
//  }



  /*char ch = fgetc(file);
  while (ch != EOF) {
    prevCharEmpty = true;
    while (ch != '\n') {
      if (isalnum(ch)) {
        prevCharEmpty = false;
      } else if
    }


  }
  while (ch != EOF) {
    prevCharEmpty = true;

    while (ch != '\n') {
      if (isalnum(ch)) {
        prevCharEmpty = false;
      } else if (ch == ':') {
        prevCharEmpty = false;
        (*countLabel)++;
      }
      prevCharEmpty = 0;
      strings[stringIndex][indexInsideString] = (char) ch;
      indexInsideString++;
      ch = fgetc(file);
    }
    if (!prevCharEmpty) {
      strings[stringIndex][indexInsideString] = '\0';
      stringIndex++;
    }
    ch = fgetc(file);
  }*/
//strings[stringIndex][indexInsideString] = '\0';


//  int ch;
//  while ((ch = fgetc(file)) != EOF) {
//    if (ch == '\n') {
//      if (!prevCharNewline) {
//        strings[stringIndex][indexInsideString] = '\0';
//        stringIndex++;
//      }
//      prevCharNewline = true;
//      prevCharEmpty = true;
//    } else if (isspace(ch)) {
//      if (!prevCharEmpty) {
//        strings[stringIndex][indexInsideString] = (char) ch;
//      }
//      prevCharEmpty = true;
//    }
//    else {
//      if (ch == ':') {
//        (*countLabel)++;
//      }
//      prevCharEmpty = false;
//      prevCharNewline = false;
//      strings[stringIndex][indexInsideString] = (char) ch;
//      indexInsideString++;
//    }
//  }
//
//  fclose(file);
//  return strings;
//}

void makeSymbolTable(row *table, int lineNum, char **lines) {
  int j = 0;
  int addr = 0;
  for (int i = 0; i < lineNum; i++) {
    if (containColon(lines[i])) {
      table[j].address = addr*4;
      deleteColon(lines[i], strlen(lines[i]));
      table[j].label = lines[i];
      j++;
    } else {
      addr++;
    }
  }
}
