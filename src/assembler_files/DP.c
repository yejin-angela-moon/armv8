#include "DP.h"

/*static char getSH(char **tokens, int numTokens) {
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
        *opc = opcWideMove(opcode);
        char* hw = numTokens == 3 ? "00" : decToBinary(stringToNumber(tokens[4]) / 16, 2);
        char* imm16 = stringToBinary(tokens[2], 16); // ???

        strcat(operand, hw);
        strcat(operand, imm16);
        if (numTokens != 3) {
            free(hw);
        }
        free(imm16);
    } else { // arithmetic
        opi = "010";
        *opc = opcArithmetic(opcode);
        char* sh = numTokens == 3 ? "0" : "1";
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
    char* opr = (char *) malloc(4 * sizeof(char));
    char* rm = registerToBinary(tokens[3]);
    char *operand = (char *) malloc(6 * sizeof(char));
    assert(operand != NULL);
    char* rn = registerToBinary(tokens[2]);
    char* rd = registerToBinary(tokens[1]);

    char *res = (char *) malloc(32 * sizeof(char));
    assert(res != NULL);

    if (strcmp("madd", opcode) == 0 || strcmp("msub", opcode) == 0) {
        operand[0] = strcmp("madd", opcode) == 0 ? '0' : '1';
        strcat(operand, registerToBinary(tokens[4]));
        M = "1";
        opr = "1000";
    } else {
        char* shiftCode = numTokens > 4 ? getShiftCode(tokens[4]) : "00";
        char* N;

        operand = stringToBinary(tokens[5], 6);
        M = "0";
        if (strcmp("and", opcode) == 0) {
            opc = "00";
            strcat(opr, "0");
            N = "0";
        } else if (strcmp("bic", opcode) == 0) {
            opc = "00";
            strcat(opr, "0");
            N = "1";
        } else if (strcmp("orr", opcode) == 0) {
            opc = "01";
            strcat(opr, "0");
            N = "0";
        } else if (strcmp("orn", opcode) == 0) {
            opc = "01";
            strcat(opr, "0");
            N = "1";
        } else if (strcmp("eon", opcode) == 0) {
            opc = "10";
            strcat(opr, "0");
            N = "0";
        } else if (strcmp("eor", opcode) == 0) {
            opc = "10";
            strcat(opr, "0");
            N = "1";
        } else if (strcmp("ands", opcode) == 0) {
            opc = "11";
            strcat(opr, "0");
            N = "0";
        } else if (strcmp("bics", opcode) == 0) {
            opc = "11";
            strcat(opr, "0");
            N = "1";
        } else { // arithmetic
            opc = opcArithmetic(opcode);
            strcat(opr, "1");
            N = "0";
        }

        strcat(opr, shiftCode);
        strcat(opr, N);
    }

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

    free(opr);
    free(operand);
    return res;
}

static char* DP(char* tokens[], int numTokens) {
    if (isRegister(tokens[3])) {
        return DPReg(tokens, numTokens);
    } else {
        return DPImm(tokens, numTokens);
    }
}
