#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "assembler_files/definition.h"
#include "assembler_files/ioutils.h"
#include "assembler_files/utilities.h"
#include "assembler_files/DP.h"
#include "assembler_files/B.h"

const char *dpSet[] = {"add", "adds", "sub", "subs"};

void parse(row *table, int numLine, char **lines, char *outputFile) {

    FILE *outFile = fopen(outputFile, "w");

    uint32_t outputLine;
    for (int i = 0; i < numLine; i++) {

        int numToken = 0;
        char **tokens = tokenizer(lines[i], &numToken);

        if (strcmp(tokens[0], "nop") == 0) {
            uint32_t instruction = 0xd503201F;
        } else if (isStringInSet(tokens[0], dpSet, dpSetSize)) {
            //DPI
            char *line = DP(tokens, numToken);
        } else if (1) {
            //SDT
        } else if ((strcmp(tokens[0], "b") == 0) || (strcmp(tokens[0], "br") == 0) || (strcmp(strncat("", token[0], 2), "b.") == 0)) {
            //B
            outputLine = B(table, tokens);
        } else if ((strcmp(tokens[0], ".int") == 0)) {
            char tyoe[] = "";
            strncpy(tyoe, tokens[1], 2);
            if (strcmp(tyoe, "0x") == 0) {
                outputLine = strtol(tokens[1], NULL, 16) == 0);
            } else {
                outputLine = atoi(tokens[1]);
            }
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

    int numLine = count_lines(inputFile);

    int countLabel = 0;
    char **lines = readFile(numLine, &countLabel, inputFile);

    row *symbol_table = malloc(sizeof(row) * countLabel);

    parse(symbol_table, numLine, lines, outputFile);

    free(symbol_table);
    freeLines(lines, numLine);

}
