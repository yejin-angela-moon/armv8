#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "assembler_files/definition.h"
#include "assembler_files/ioutils.h"
#include "assembler_files/utilities.h"

void parse(row *table, int numLine, char **lines, char *outputFile) {

  File *outFile = fopen(outputFile, "w");

  for (int i = 0; i < numLine; i++) {

    int numToken = 0;
    char **token = tokenizer(lines[i], &numToken);

    if (strcmp(token[1], "nop") == 0) {
      uint32_t instruction = 0xd503201F;
    } else if (1) {
      //DPI
    } else if (1) {
      //DPR
    } else if (1) {
      //SDT
    } else if (1) {
      //B
    } else {
      //not a instruction
    }

    fprintf(outFile, _, _);

    free(token);
  }
  //Either print as soon as it gets to the instruction
  //Or store in an output String array then output

}


int main(int argc, char **argv) {
  assert(argc == 3);

  char *inputFile = argv[1];
  char *outputFile = argv[2];

  int numLine = count_line(inputFile);

  int countLabel = 0;
  char **lines = readFile(inputFile, numLine, &countLabel);

  row *symbol_table = malloc(sizeof(row) * countLabel);

  parse(symbol_table, numLine, lines);

  free(symbol_table);
  freeLines(lines, numLine);

}
