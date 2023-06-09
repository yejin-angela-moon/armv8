#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "DP.h"

static char getSH(char **tokens, int numTokens) {
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

/*uint32_t arithmetic(char* tokens[], int numTokens) {
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

}*/

char* DPImm(char* tokens[], int numTokens) {
    char* opcode = tokens[0];
    char* sf = getSF(tokens[1]);
    char* opi;
    char* opc;
    char *operand = (char *) malloc(16 * sizeof(char));
    assert(operand != NULL);
    char* rd = registerToBinary(tokens[1]);

    char *res = (char *) malloc(32 * sizeof(char));
    assert(res != NULL);

    if (strcmp("movk", opcode) == 0 || strcmp("movn", opcode) == 0 || strcmp("movz", opcode) == 0) {
        opi = "101";
        if (strcmp("movk", opcode) == 0) {
            opc = "11";
        } else if (strcmp("movn", opcode) == 0) {
            opc = "00";
        } else {
            opc = "10";
        }
        char* hw = numTokens == 3 ? "00" : decToBinary(atoi(tokens[4]) / 16, 2);
        char* imm16 = stringToBinary(tokens[2], 16); // ???

        strcat(operand, hw);
        strcat(operand, imm16);
        if (numTokens != 3) {
            free(hw);
        }
        free(imm16);
    } else if (strcmp("add", opcode) == 0 || strcmp("adds", opcode) == 0 || strcmp("sub", opcode) == 0 || strcmp("subs", opcode) == 0) {
        opi = "010";
        if (strcmp("add", opcode) == 0) {
            opc = "00";
        } else if (strcmp("adds", opcode) == 0) {
            opc = "01";
        } else if (strcmp("adds", opcode) == 0) {
            opc = "10";
        } else {
            opc = "11";
        }
        char sh = numTokens == 3 ? "0" : "1";
        char* imm12 = stringToBinary(tokens[3], 12);
        char* rn = registerToBinary(tokens[2]);

        strcat(operand, sh);
        strcat(operand, imm12);
        strcat(operand, rn);

        free(sh);
        free(imm12);
        free(rn);
    }

    strcat(res, sf);
    strcat(res, opc);
    strcat(res, "100");
    strcat(res, opi);
    strcat(res, operand);
    strcat(res, rd);
    strcat(res, "\0");

    free(operand);
    free(rd);
    return res;
}

char* DPReg(char* tokens[], int numTokens) {
    char* opcode = tokens[0];
    char* sf = getSF(tokens[1]);
    char* opc;
    char* M;
    char* opr;
    char* rm;
    char *operand;
    char* rn;
    char* rd = registerToBinary(tokens[1]);

    char *res = (char *) malloc(32 * sizeof(char));
    assert(res != NULL);

    strcat(res, sf);
    strcat(res, opc);
    strcat(res, M);
    strcat(res, "101");
    strcat(res, opr);
    strcat(res, rm);
    strcat(res, operand);
    strcat(res, rn);
    strcat(res, rd);
    strcat(res, "\0");
    return res;
}

char* DP(char* tokens[], int numTokens) {
  if (isRegister(tokens[2])) {
    return DPReg(tokens, numTokens);
  } else {
    return DPImm(tokens, numTokens);
  }
}
