#include <stdlib.h>
#include <assert.h>
#include "assembler_files/definition.h"
#include "assembler_files/ioutils.h"
#include "assembler_files/utilities.h"
#include "assembler_files/DP.h"

static uint32_t DP(char* tokens[], int numOfTokens);
const char *dpSet[] = {"add", "adds", "sub", "subs"};

void parse(row *table, int numLine, char **lines){

  for (int i =0; i < numLine; i++){
    if (isStringInSet(tokens[0], dpSet*, sizeof(dpSet))){
        //DPI
      } else if(1){
        //DPR
      } else if(1){
        //SDT
      } else if (1){
        //B
      } else {
        //not a instruction
      }
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
  char** lines = readFile(inputFile, numLine, &countLabel);

  row *symbol_table = malloc(sizeof(row) * countLabel);

  parse(symbol_table, numLine, lines);

  free(symbol_table);
  freeLines(lines, numLine);

}
