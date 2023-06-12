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
 if (strcmp("cmp", opcode) == 0) {
   (*numToken)++;
   newTokens[0] = "subs";
   newTokens[1] = "rzr";
   newTokens[2] = tokens[1];
   newTokens[3] = tokens[2];
 } else if (strcmp("cmn", opcode) == 0) {
   (*numToken)++;
   newTokens[0] = "adds";
   newTokens[1] = "rzr";
   newTokens[2] = tokens[1];
   newTokens[3] = tokens[2];
 } else if (strcmp("neg", opcode) == 0) {
   (*numToken)++;
   newTokens[0] = "sub";
   newTokens[1] = tokens[1];
   newTokens[2] = "rzr";
   newTokens[3] = tokens[2];
 } else if (strcmp("negs", opcode) == 0) {
   (*numToken)++;
   newTokens[0] = "subs";
   newTokens[1] = tokens[1];
   newTokens[2] = "rzr";
   newTokens[3] = tokens[2];
 } else if (strcmp("tst", opcode) == 0) {
   (*numToken)++;
   newTokens[0] = "ands";
   newTokens[1] = "rzr";
   newTokens[2] = tokens[1];
   newTokens[3] = tokens[2];
 } else if (strcmp("mvn", opcode) == 0) {
   (*numToken)++;
   newTokens[0] = "orn";
   newTokens[1] = tokens[1];
   newTokens[2] = "rzr";
   newTokens[3] = tokens[2];
 } else if (strcmp("mov", opcode) == 0) {
   (*numToken)++;
   newTokens[0] = "orr";
   newTokens[1] = tokens[1];
   newTokens[2] = "rzr";
   newTokens[3] = tokens[2];
 } else if (strcmp("mul", opcode) == 0) {
   (*numToken)++;
   newTokens[0] = "madd";
   newTokens[1] = tokens[1];
   newTokens[2] = tokens[2];
   newTokens[3] = tokens[3];
   newTokens[4] = "rzr";
 } else if (strcmp("mneg", opcode) == 0) {
   (*numToken)++;
   newTokens[0] = "msub";
   newTokens[1] = tokens[1];
   newTokens[2] = tokens[2];
   newTokens[3] = tokens[3];
   newTokens[4] = "rzr";
 } else {
   newTokens = tokens;
 }
 return newTokens;
}

void parse(row *table, int numLine, char **lines, char *outputFile) {
 printf("start phrase ");
 uint32_t currAddress = 0;
 printf("line no %d: \n", numLine);

 FILE *outFile = fopen(outputFile, "w");
 rewind(outFile);
 if (outFile == NULL) {
   printf("error on opening");
 }
 printf("file opened\n");
 for (int i = 0; i < numLine - 1; i++) {

   printf("line: %d\n", i);

   int numToken = 0;
   printf("ready to take from %s\n", lines[i]);
   char **tokens = tokenizer(lines[i], &numToken);
   printf("token\n");

   tokens = alias(tokens, &numToken);
   printf("opcode:%s ", tokens[0]);

   char *opcode = tokens[0];
   printf("opcode:%s ", opcode);

   if (isStringInSet(opcode, dpSet, dpSetSize)) {
     currAddress += 4;
     fprintf(outFile, "%i", stringToNumber(DP(tokens, numToken)));
   } else if (isStringInSet(opcode, sdtSet, sdtSetSize)) {
     currAddress += 4;
     fprintf(outFile, "%x", SDT(tokens, table, numToken, currAddress));
   } else if (opcode[0] == 'b') {
     //printf("b instr");
     //uint32_t instr = B(table, tokens, &currAddress);
     //printf("instr: %x\n", B(table, tokens, &currAddress));
     //rewind(outFile);
     fprintf(outFile, "%x\n", B(table, tokens, &currAddress));
     //fflush(outFile);
     //printf("b end");
   } else if (strcmp("nop", opcode) == 0) {
     currAddress += 4;
     //printf("instr: %x\n", NOP_INSTRUCTION);
     fprintf(outFile, "%x\n", NOP_INSTRUCTION);
   } else if (strcmp(".int", opcode) == 0) {
     fprintf(outFile, "%x", stringToNumber(tokens[1]));
   }
   numToken = 1;
    //printf("before token %s +  %s +  %s\n ", tokens[0], tokens[1], tokens[2]);
    for (int i = 0; i < MAX_TOKEN; i++) {
        free(tokens[i]);
      }
   free(tokens);
   //printf("%d", numToken);
  // for (int i = 0; i < numToken; i ++) {
    // tokens[i] = NULL;
   //}
    printf("after token %s +  %s +  %s\n ", tokens[0], tokens[1], tokens[2]);
 }
 fclose(outFile);
}

int main(int argc, char **argv) {
 assert(argc == 3);
 printf("start\n");

 char *inputFile = argv[1];
 char *outputFile = argv[2];

 int numLine = count_lines(inputFile);

 int countLabel = 0;
 char **lines = readFile(numLine, &countLabel, inputFile);
 printf("ready to row\n");

 row *symbol_table = malloc(sizeof(row) * countLabel);
 printf("ready to symbol\n");
 makeSymbolTable(symbol_table, numLine, lines);
  printf("ready to parse\n");
 parse(symbol_table, numLine, lines, outputFile);
 printf("exit\n");
 free(symbol_table);
 printf("free table\n");
 freeLines(lines, numLine);
 printf("free lines\n");

 return EXIT_SUCCESS;
}
