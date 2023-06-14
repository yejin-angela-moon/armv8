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
    newTokens[1] = getZeroRegister(tokens[1]);
    for (int i = 2; i < *numToken + 1; i++) {
      newTokens[i] = tokens[i - 1];
    }
  } else if (strcmp("cmn", opcode) == 0) {
    newTokens[0] = "adds";
    newTokens[1] = getZeroRegister(tokens[1]);
    for (int i = 2; i < *numToken + 1; i++) {
      newTokens[i] = tokens[i - 1];
    }
  } else if (strcmp("neg", opcode) == 0) {
    newTokens[0] = "sub";
    newTokens[1] = tokens[1];
    newTokens[2] = getZeroRegister(tokens[1]);
    newTokens[3] = tokens[2];
  } else if (strcmp("negs", opcode) == 0) {
    newTokens[0] = "subs";
    newTokens[1] = tokens[1];
    newTokens[2] = getZeroRegister(tokens[1]);
    newTokens[3] = tokens[2];
  } else if (strcmp("tst", opcode) == 0) {
    newTokens[0] = "ands";
    newTokens[1] = getZeroRegister(tokens[1]);
    for (int i = 2; i < *numToken + 1; i++) {
      newTokens[i] = tokens[i - 1];
    }
  } else if (strcmp("mvn", opcode) == 0) {
    newTokens[0] = "orn";
    newTokens[1] = tokens[1];
    newTokens[2] = getZeroRegister(tokens[1]);
    newTokens[3] = tokens[2];
  } else if (strcmp("mov", opcode) == 0) {
    newTokens[0] = "orr";
    newTokens[1] = tokens[1];
    newTokens[2] = getZeroRegister(tokens[1]);
    newTokens[3] = tokens[2];
  } else if (strcmp("mul", opcode) == 0) {
    newTokens[0] = "madd";
    newTokens[1] = tokens[1];
    newTokens[2] = tokens[2];
    newTokens[3] = tokens[3];
    newTokens[4] = getZeroRegister(tokens[1]);
  } else if (strcmp("mneg", opcode) == 0) {
    newTokens[0] = "msub";
    newTokens[1] = tokens[1];
    newTokens[2] = tokens[2];
    newTokens[3] = tokens[3];
    newTokens[4] = getZeroRegister(tokens[1]);
  } else {
    free(newTokens);
    return tokens;
  }
  (*numToken)++;
  free(tokens);
  return newTokens;
}

void parse(row *table, int numLine, char **lines, char *outputFile) {
  uint32_t currAddress = 0;

  FILE *outFile = fopen(outputFile, "wb");
  assert(outFile != NULL);

  for (int i = 0; i < numLine; i++) {

    int numToken = 0;
    char **tokens = malloc(MAX_TOKEN * sizeof(char*));
    tokens = tokenizer(lines[i], &numToken, tokens);
    //printf("tokens %s + %s + %s + %s + %s + %s\n ", tokens[0], tokens[1], tokens[2], tokens[3], tokens[4], tokens[5]);
  // printf("tokens %s + %s + %s + %s + %s + %s\n ", tokens[0], tokens[1], tokens[2], tokens[3], tokens[4], tokens[5]);
   if (tokens[0] == NULL) {
     tokens[0] = "";
   }
    tokens = alias(tokens, &numToken);
   // printf("tokens %s + %s + %s + %s + %s + %s\n ", tokens[0], tokens[1], tokens[2], tokens[3], tokens[4], tokens[5]);
    char *opcode = tokens[0];
    uint32_t result;

  //  char *store_DP = (char *) malloc(33 * sizeof(char));
   // assert(store_DP != NULL);

    if (isStringInSet(opcode, dpSet, dpSetSize)) {
      //strcpy(store_DP, DP(tokens, numToken));
      result = DP(tokens, numToken);
     // printf("DP = %s||\n", store_DP);
     //// result = strtoll(store_DP, NULL, 2);
     // char* bin = "10011011000011000000010001110000";
      // printf("result = %x\n", strtoll(bin, NULL, 2));
      //result = binaryStringToNumber(DP(tokens, numToken));
      currAddress += 4;
    } else if (isStringInSet(opcode, sdtSet, sdtSetSize)) {
      result = SDT(tokens, table, numToken, currAddress);
      currAddress += 4;
    } else if (opcode[0] == 'b') {
      result = B(table, tokens, &currAddress);
      currAddress += 4;
    } else if (strcmp("nop", opcode) == 0) {
      result = NOP_INSTRUCTION;
      currAddress += 4;
    } else if (strcmp(".int", opcode) == 0) {
      if (strchr(tokens[1],'x') != NULL) {
        result = strtol(tokens[1], NULL, 16);
      } else {
        result = strtol(tokens[1], NULL, 10);
      }
      currAddress += 4;
    } else {
      //label
      //free(tokens);
      continue;
    }

    printf("%x\n", result);
    fwrite(&result, sizeof(int32_t), 1, outFile);
    free(tokens);
    //free(store_DP);
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
