#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "utilities.h"

char** readFile(int lineNum, int *countLabel ,char *filename) {
  FILE *fp = fopen(filename, "r");

  if (fp == NULL) {
    fprintf(stderr, "can't open %s\n", filename);
    exit(1);
  }

  char buffer[MAX_LINE_LENGTH];
  char **strings = calloc(lineNum, sizeof(char*));

  for (int i = 0; i < lineNum; i++){
    if (fgets(buffer, sizeof(buffer), fp) != NULL) {
      if(isspace(buffer[0])) {
        continue;
      }
      if (containColon(buffer)){
        (*countLabel)++;
      }

      buffer[strcspn(buffer, "\n")] ='\0';
      strings[i] = (char*)calloc(MAX_LINE_LENGTH, sizeof(char));

      strcpy(strings[i], buffer);
    }
  }

  fclose(fp);
  return strings;
}

void makeSymbolTable(row *table, int lineNum, char **lines){
  int tableIndex = 0;
  for (int i = 0; i < lineNum; i++){
    if (containColon(lines[i])){
      // Extract the label name from the line (assuming it's up to the colon)
      char *labelEnd = strchr(lines[i], ':');
      int labelLength = labelEnd - lines[i];
      table[tableIndex].label = malloc(labelLength + 1);
      strncpy(table[tableIndex].label, lines[i], labelLength);
      table[tableIndex].label[labelLength] = '\0';

      // Assign the address (assuming each instruction is 4 bytes)
      table[tableIndex].address = i * 4;

      // Move to the next position in the table
      tableIndex++;
    }
  }
}

