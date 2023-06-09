#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "assembler_files/definition.h"
#include "assembler_files/ioutils.h"
#include "assembler_files/utilities.h"

const char *dpSet[] = {"add", "adds", "sub", "subs"};

void parse(row *table, int numLine, char **lines, char *outputFile) {

  FILE *outFile = fopen(outputFile, "w");

  uint32_t outputLine;

  for (int i = 0; i < numLine; i++) {

    int numToken = 0;
    char **tokens = tokensizer(lines[i], &numToken);

    if (isStringInSet(tokens[0], dpSet, dpSetSize)) {
      //DPI
      outputLine = DP(tokens, numToken);
    } else if (1) {
      //DPR
    } else if (1) {
      //SDT
    } else if (1) {
      //B
    } else {
      //not a instruction
    }

    fprintf(outFile, "%x\n", outputLine);

    free(tokens);
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

  parse(symbol_table, numLine, lines, outputFile);

  free(symbol_table);
  freeLines(lines, numLine);

}
