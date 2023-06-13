#include "DP.h"

static char* opcArithmetic(const char* opcode) {
  if (strcmp("add", opcode) == 0) {
    return "00";
  } else if (strcmp("adds", opcode) == 0) {
    return "01";
  } else if (strcmp("sub", opcode) == 0) {
    return "10";
  } else if (strcmp("subs", opcode) == 0) {
    return "11";
  } else {
    fprintf(stderr, "invalid opcode\n");
    exit(1);
  }
}

static char* opcWideMove(const char* opcode) {
  if (strcmp("movk", opcode) == 0) {
    return "11";
  } else if (strcmp("movn", opcode) == 0) {
    return "00";
  } else if (strcmp("movz", opcode) == 0) {
    return "10";
  } else {
    fprintf(stderr, "invalid opcode\n");
    exit(1);
  }
}

static char* getShiftCode(char* shift) {
  if (strcmp("lsl", shift) == 0) {
    return "00";
  } else if (strcmp("lsr", shift) == 0) {
    return "01";
  } else if (strcmp("asr", shift) == 0) {
    return "10";
  } else if (strcmp("ror", shift) == 0) {
    return "11";
  } else {
    fprintf(stderr, "invalid shift name\n");
    exit(1);
  }
}


char* DPImm(char* tokens[], int numTokens) {
    char* opcode = tokens[0];
    char* sf = getSF(tokens[1]);
    char* opi;
    char* opc;
    char *operand = (char *) malloc(18 * sizeof(char));
    assert(operand != NULL);
    char* rd = registerToBinary(tokens[1]);

    char res[32];
    //assert(res != NULL);

     if (strcmp("movk", opcode) == 0 || strcmp("movn", opcode) == 0 || strcmp("movz", opcode) == 0) {
        opi = "101";
        opc = opcWideMove(opcode);
        char* hw = numTokens == 3 ? "00" : decToBinary(stringToNumber(tokens[4]) / 16, 2);
        char* imm16 = stringToBinary(tokens[2], 16); // ???

        strcpy(operand, hw);
        strcat(operand, imm16);
        if (numTokens != 3) {
          free(hw);
        }
        free(imm16);

    } else { // arithmetic
        opi = "010";
        opc = opcArithmetic(opcode);
        //printf("num of tokens %d", numTokens);
        char* sh = numTokens - 1 == 3 ? "0" : "1";
        char* imm12 = stringToBinary(tokens[3], 12);
        char* rn = registerToBinary(tokens[2]);

        strcpy(operand, sh);
        strcat(operand, imm12);
        strcat(operand, rn);

       // printf("operand = %s\n", operand);

        //free(sh);
        free(imm12);
        free(rn);
    }


    strcpy(res, sf);
    strcat(res, opc);
    strcat(res, "100");
    strcat(res, opi);
    strcat(res, operand);
    strcat(res, rd);
    strcat(res, "\0");


    //printf("before free %s", operand);
    free(operand);
    //printf("after free %s", operand);
    //printf("res = %s\n", res);
    //free(rd);
    char *result  = (char *) malloc(32 * sizeof(char));
    strncpy(result, res, 32);
    return result;
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

    char res[32];
   // assert(res != NULL);

    if (strcmp("madd", opcode) == 0 || strcmp("msub", opcode) == 0) {
        operand[0] = strcmp("madd", opcode) == 0 ? '0' : '1';
        strcat(operand, registerToBinary(tokens[4]));
        M = "1";
        opr = "1000";
    } else {
        char* shiftCode = numTokens > 4 ? getShiftCode(tokens[4]) : "00";
        char* N;

        operand = numTokens > 4 ? stringToBinary(tokens[5], 6) : "000000";
        M = "0";
        if (strcmp("and", opcode) == 0) {
            opc = "00";
            //printf("opc = %s\n", opc);
            strcpy(opr, "0");
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

    strcpy(res, sf);
    strcat(res, opc);
    strcat(res, M);
    strcat(res, "101");
    strcat(res, opr);
    strcat(res, rm);
    strcat(res, operand);
    strcat(res, rn);
    strcat(res, rd);
    strcat(res, "\0");

    //printf("res = %s\n", res);

    free(opr);
    if (strcmp("madd", opcode) == 0 || strcmp("msub", opcode) == 0 || numTokens > 4) {
        free(operand);
    }
    char *result  = (char *) malloc(32 * sizeof(char));
    strncpy(result, res, 32);
    return result;
}

char* DP(char* tokens[], int numTokens) {
    if (isRegister(tokens[3])) {
        return DPReg(tokens, numTokens);
    } else {
        return DPImm(tokens, numTokens);
    }
}
