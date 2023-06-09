#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "DP.h"

char getSH(char **tokens, int numTokens) {
  if (numTokens == 3) {
    return 0;
  } else {
    char shiftType[3]; // buffer to store the shiftType
    int shiftAmount;          // variable to store the shiftAmount
    assert(sscanf(tokens[4], "%s #%d", shiftType, &shiftAmount) == 2);
    if (shiftAmount == 0) {
      return 0;
    } else {
      return 1;
    }
  }
}

uint32_t arithmetic(char* tokens[], int numTokens) {
  char sf = getSF(tokens[1]); // bit 31
  char* opc; // bits 30 to 29
  char* rd = registerToBinary(tokens[1]); // bits 4 to 0

  char* bits22to5;

  if (isRegister(tokens[3])) {
    // DPReg
    char* bits28to25 = "0101"; // M and dpr identifier
    char* bits15to10;
    char* rm = registerToBinary(tokens[3]); // bits 20 to 16
    char* rn = registerToBinary(tokens[2]); // bits 9 to 5

    if (numTokens == 3) {
      // no shift
      bits15to10 = "0000";
    } else {
      // shift
      char* bits24to21; // opr

      char shiftType[3]; // buffer to store the shiftType
      int shiftAmount;          // variable to store the shiftAmount

      assert(sscanf(tokens[4], "%s #%d", shiftType, &shiftAmount) == 2);
      if (strcmp(shiftType, "lsl") == 0) {
        bits24to21 = "1000";
      } else if (strcmp(shiftType, "lsr") == 0) {
        bits24to21 = "1010";
      } else if (strcmp(shiftType, "asr") == 0) {
        bits24to21 = "1100";
      }
      bits15to10 = decToBinary(shiftAmount);
    }
  } else {
    // DPImm
    char* bits28to23 = "100010";
    char bit22 = getSH(tokens, numTokens); // sh, 1 if shift
    int immediate;
    assert(sscanf(tokens[3], "#%d", immediate) == 1);
    char* bits21to10 = decToBinary(immediate);
    char* rn = registerToBinary(tokens[1]);
  }

}

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
