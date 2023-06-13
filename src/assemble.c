#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
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
 char buffer[8];

 FILE *outFile = fopen(outputFile, "wb");
 //rewind(outFile);
 if (outFile == NULL) {
   printf("error on opening");
 }
 printf("file opened\n");
 typedef char word[60];

 char **tokens = calloc(sizeof(word), MAX_TOKEN);
 if (tokens == NULL) {
   printf("error on allocating mem");
 }
 //printf("mem allocing");
 for (int i = 0; i < MAX_TOKEN; i++) {
   //printf("get in loop");
   //token[i] = malloc( sizeof(word) );
   if ((tokens[i] = calloc( sizeof(char), 60))== NULL) {
       printf("error on allocating mem for token[%d]", i);
     }
 }

 printf("mem alloc\n");
 char * result;
 for (int i = 0; i < numLine - 1; i++) {

   printf("line: %d\n", i);

   int numToken = 0;
   printf("ready to take from %s\n", lines[i]);
   tokens = tokenizer(lines[i], &numToken, tokens);
   //printf("token\n");

   tokens = alias(tokens, &numToken);
  // printf("opcode:%s ", tokens[0]);

   char *opcode = tokens[0];
   printf("opcode:%s \n", opcode);

   uint32_t instruction = 0;

   if (isStringInSet(opcode, dpSet, dpSetSize)) {
     //printf("dp since the op is %s", tokens[0]);
     //printf("number of token = %d", numToken);
     currAddress += 4;

     result = malloc( sizeof(char)* 32);
     if (result == NULL) {
       printf("fail on allocating result");
     }

     strncpy(result, DP(tokens, numToken), 32);
     //printf("print result %s\n", result);
     uint32_t instr = strtoll(result, NULL, 2);
     //free(str);
     //printf("instr = %x", instr);
     //fprintf(outFile, "%x\n", instr);
     instruction = instr;
     //printf("print in file %s\n", result);
     free(result);


   } else if (isStringInSet(opcode, sdtSet, sdtSetSize)) {
     currAddress += 4;
     //fprintf(outFile, "%x", SDT(tokens, table, numToken, currAddress));
     instruction = SDT(tokens, table, numToken, currAddress);
   } else if (opcode[0] == 'b') {
     currAddress += 4;
     //fprintf(outFile, "%x\n", B(table, tokens, &currAddress));
     instruction = B(table, tokens, &currAddress);
     //fflush(outFile);
     //printf("b end");
   } else if (strcmp("nop", opcode) == 0) {
     currAddress += 4;
     //printf("instr: %x\n", NOP_INSTRUCTION);
     //fprintf(outFile, "%x\n", NOP_INSTRUCTION);
     instruction = NOP_INSTRUCTION;
   } else if (strcmp(".int", opcode) == 0) {
     //fprintf(outFile, "%x", stringToNumber(tokens[1]));
     instruction = stringToNumber(tokens[1]);
   }
   // printf("before token %s +  %s +  %s+  %s\n ", tokens[0], tokens[1], tokens[2], tokens[3]);

   //printf("%d", numToken);
   for (int i = 0; i < numToken + 1; i ++) {
     free(tokens[i]);
     tokens[i] = calloc( sizeof(char), 60);
   }
    //printf("after token %s +  %s +  %s+  %s\n ", tokens[0], tokens[1], tokens[2], tokens[3]);
   // itoa(instruction, buffer, 1);
    sprintf(buffer, "%u", instruction);
    printf("buffer = %s", buffer);
    fwrite(buffer,sizeof(buffer),1,outFile);
 }
    for (int i = MAX_TOKEN; i > 0; i--) {
         free(tokens[i]);
     }
     //free(tokens[3]);
    free(tokens);
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
 //printf("ready to row\n");

 row *symbol_table = malloc(sizeof(row) * countLabel);
 //printf("ready to symbol\n");
 makeSymbolTable(symbol_table, numLine, lines);
  //printf("ready to parse\n");
 parse(symbol_table, numLine, lines, outputFile);
 //printf("exit\n");
 free(symbol_table);
 //printf("free table\n");
 freeLines(lines, numLine);
 printf("free lines\n");

 return EXIT_SUCCESS;
}
