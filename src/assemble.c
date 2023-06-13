#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "assembler_files/definition.h"
#include "assembler_files/ioutils.h"
#include "assembler_files/utilities.h"
#include "assembler_files/DP.h"
#include "assembler_files/B.h"
#include "assembler_files/SDT.h"

char *dpSet[] = {"add", "adds", "sub", "subs", "movn", "movz", "movk", "and", "bic", "orr", "orn", "eon", "eor", "ands",
                 "bics", "madd", "msub"};
char *sdtSet[] = {"ldr", "str"};
#define dpSetSize 17
#define sdtSetSize 2

static char **alias(char **tokens, int *numToken) {
  char *opcode = tokens[0];
  char **newTokens = calloc(*numToken + 1, sizeof(tokens));
  assert(newTokens != NULL);
  if (strcmp("cmp", opcode) == 0) {
    newTokens[0] = "subs";
    newTokens[1] = "rzr";
    newTokens[2] = tokens[1];
    newTokens[3] = tokens[2];
  } else if (strcmp("cmn", opcode) == 0) {
    newTokens[0] = "adds";
    newTokens[1] = "rzr";
    newTokens[2] = tokens[1];
    newTokens[3] = tokens[2];
  } else if (strcmp("neg", opcode) == 0) {
    newTokens[0] = "sub";
    newTokens[1] = tokens[1];
    newTokens[2] = "rzr";
    newTokens[3] = tokens[2];
  } else if (strcmp("negs", opcode) == 0) {
    newTokens[0] = "subs";
    newTokens[1] = tokens[1];
    newTokens[2] = "rzr";
    newTokens[3] = tokens[2];
  } else if (strcmp("tst", opcode) == 0) {
    newTokens[0] = "ands";
    newTokens[1] = "rzr";
    newTokens[2] = tokens[1];
    newTokens[3] = tokens[2];
  } else if (strcmp("mvn", opcode) == 0) {
    newTokens[0] = "orn";
    newTokens[1] = tokens[1];
    newTokens[2] = "rzr";
    newTokens[3] = tokens[2];
  } else if (strcmp("mov", opcode) == 0) {
    newTokens[0] = "orr";
    newTokens[1] = tokens[1];
    newTokens[2] = "rzr";
    newTokens[3] = tokens[2];
  } else if (strcmp("mul", opcode) == 0) {
    newTokens[0] = "madd";
    newTokens[1] = tokens[1];
    newTokens[2] = tokens[2];
    newTokens[3] = tokens[3];
    newTokens[4] = "rzr";
  } else if (strcmp("mneg", opcode) == 0) {
    newTokens[0] = "msub";
    newTokens[1] = tokens[1];
    newTokens[2] = tokens[2];
    newTokens[3] = tokens[3];
    newTokens[4] = "rzr";
  } else {
    free(newTokens);
    return tokens;
  }
  (*numToken)++;
  return newTokens;
}

void parse(row *table, int numLine, char **lines, char *outputFile) {
  uint32_t currAddress = 0;

  FILE *outFile = fopen(outputFile, "wb");
  assert(outFile != NULL);

  for (int i = 0; i < numLine; i++) {

    int numToken = 0;
    char **tokens = tokenizer(lines[i], &numToken);

    tokens = alias(tokens, &numToken);

    char *opcode = tokens[0];

    uint32_t result;

    if (isStringInSet(opcode, dpSet, dpSetSize)) {
      currAddress += 4;
      result = binaryStringToNumber(DP(tokens, numToken));
    } else if (isStringInSet(opcode, sdtSet, sdtSetSize)) {
      currAddress += 4;
      result = SDT(tokens, table, numToken, currAddress);
    } else if (opcode[0] == 'b') {
      result = B(table, tokens, &currAddress);
    } else if (strcmp("nop", opcode) == 0) {
      currAddress += 4;
      result = NOP_INSTRUCTION;
    } else if (strcmp(".int", opcode) == 0) {
      result = stringToNumber(tokens[1]);
    } else {
      //label
      free(tokens);
      continue;
    }

    fwrite(&result, sizeof(uint32_t), 1, outFile);
    free(tokens);
  }
  fclose(outFile);
}

int main(int argc, char **argv) {
  assert(argc == 3);

  char *inputFile = argv[1];
  char *outputFile = argv[2];

  int numLine = count_lines(inputFile);

  int countLabel = 0;
  char **lines = readFile(numLine, &countLabel, inputFile);

  row *symbol_table = malloc(sizeof(row) * countLabel);
  makeSymbolTable(symbol_table, numLine, lines);

  parse(symbol_table, numLine, lines, outputFile);

  free(symbol_table);
  freeLines(lines, numLine);

  return EXIT_SUCCESS;
}