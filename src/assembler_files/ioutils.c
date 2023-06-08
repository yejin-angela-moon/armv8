#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "definition.h"
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
    if (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
      if(isspace(buffer[0])){
        continue;
      }
      if (containColon(buffer)){
        *countLabel++;
      }

      buffer[strcspn(buffer, "\n")] ='\0';
      strings[i] = (char*)calloc(MAX_LINE_LENGTH, sizeof(char));

      strcpy(strings[i], buffer);
    }
  }

  fclose(fp);
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
