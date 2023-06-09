#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "DP.h"

uint32_t DP(char* tokens[], int numTokens) {
    char* opcode = tokens[0];
    char* rd;
    char* sf;

    uint32_t output;

    //movz without shift means mov

    if (strcmp("movk", opcode) == 0) {
        char* opi = "101";
        char* opc = "11";
        rd = registerToBinary(tokens[1], 5);
        sf = getSF(rd);
    }

    if (strcmp("movn", opcode) == 0) {
        char* opi = "101";
        char* opc = "00";
        rd = registerToBinary(tokens[1], 5);
        sf = getSF(rd);
    }

    if (strcmp("movz", opcode) == 0) {
        char* opi = "101";
        char* opc = "10";
        rd = registerToBinary(tokens[1], 5);
        sf = getSF(rd);

        if (numTokens == 3) {

        }
    }
}
