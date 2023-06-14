#include "DP.h"
// //
// // change everything to uint32_t

static uint32_t opcArithmetic(const char* opcode) {
  if (strcmp("add", opcode) == 0) {
    return 0;
  } else if (strcmp("adds", opcode) == 0) {
    return 1;
  } else if (strcmp("sub", opcode) == 0) {
    return 2;
  } else if (strcmp("subs", opcode) == 0) {
    return 3;
  } else {
    fprintf(stderr, "invalid opcode\n");
    exit(1);
  }
}

static uint32_t opcWideMove(const char* opcode) {
  if (strcmp("movk", opcode) == 0) {
    return 0x3;
  } else if (strcmp("movn", opcode) == 0) {
    return 0;
  } else if (strcmp("movz", opcode) == 0) {
    return 0x2;
  } else {
    fprintf(stderr, "invalid opcode\n");
    exit(1);
  }
}

static uint32_t getShiftCode(char* shift) {
  if (strcmp("lsl", shift) == 0) {
    return 0;
  } else if (strcmp("lsr", shift) == 0) {
    return 1;
  } else if (strcmp("asr", shift) == 0) {
    return 2;
  } else if (strcmp("ror", shift) == 0) {
    return 3;
  } else {
    fprintf(stderr, "invalid shift name\n");
    exit(1);
  }
}

uint32_t DPImm(char** tokens, int numTokens) {
  char* opcode = tokens[0];
  uint32_t sf = strtol(getSF(tokens[1]), NULL, 2);
  uint32_t opi;
  uint32_t opc;
  //char *operand = (char *) malloc(19 * sizeof(char));
  //assert(operand != NULL);
  uint32_t operand = 0;
  uint32_t rd = strtol(registerToBinary(tokens[1]), NULL, 2);

  //char *res = (char *) malloc(33 * sizeof(char));
  //assert(res != NULL);

  if (strcmp("movk", opcode) == 0 || strcmp("movn", opcode) == 0 || strcmp("movz", opcode) == 0) {
    opi = 0x5;
    opc = opcWideMove(opcode);
    uint32_t hw = numTokens == 3 ? 0 : stringToNumber(tokens[4]) / 16;
    uint16_t imm16 = strtol(tokens[2], NULL, 16); // ???

    operand += hw << 16;
    operand += imm16;

    //strcpy(operand, hw);
    //strcat(operand, imm16);
    //if (numTokens != 3) {
     // free(hw);
    //}
    //free(imm16);
  } else { // arithmetic
    opi = 0x2;
    opc = opcArithmetic(opcode);
    //printf("num of tokens %d", numTokens);
    uint32_t sh;
    if (numTokens == 4) {
      sh = 0;
    } else if (strcmp(tokens[5], "0") == 0) {
      sh = 0;

    } else {
      sh = 1;
    }
    uint16_t imm12 = strtol(tokens[3], NULL, 16) & 0x0FFF ;
    uint32_t rn = stringToNumber(tokens[2] + 1);

    //strcpy(operand, sh);
    //strcat(operand, imm12);
    //strcat(operand, rn);
    operand += sh << 17;
    operand += imm12 << 5;
    operand += rn;

    // printf("operand = %s\n", operand);

    //free(sh);
    //free(imm12);
    //free(rn);
  }

  uint32_t res = sf << 31;
  res += opc << 29;
  res += 1 << 28;
  res += opi << 23;
  res += operand << 5;
  res += rd;
 // strcpy(res, sf);
  //strcat(res, opc);
  //strcat(res, "100");
  //strcat(res, opi);
  //strcat(res, operand);
  //strcat(res, rd);
  //strcat(res, "\0");

  // printf("res = %s\n", res);

 // free(operand);
  //free(rd);
  return res;
}

uint32_t DPReg(char** tokens, int numTokens) {
  char* opcode = tokens[0];
  uint32_t sf = strtol(getSF(tokens[1]), NULL, 2);
  uint32_t opc;
  uint32_t M;
 // uint32_t opr = (char *) malloc(5 * sizeof(char));
  uint32_t opr;
  //assert(opr != NULL);
  uint32_t rm = strtol(registerToBinary(tokens[3]), NULL, 2);
  //char *operand = (char *) malloc(7 * sizeof(char));
  uint32_t operand = 0;
  //assert(operand != NULL);
  uint32_t rn = strtol(registerToBinary(tokens[2]), NULL, 2);
  uint32_t rd = strtol(registerToBinary(tokens[1]), NULL, 2);

  //char *res = (char *) malloc(33 * sizeof(char));
  //assert(res != NULL);

  if (strcmp(opcode, "madd") == 0 || strcmp(opcode, "msub") == 0) {
    opc = 0;
    operand += (strcmp(opcode, "madd") == 0 ? 0 : 1) << 5;
   // strcpy(operand, strcmp(opcode, "madd") == 0 ? "0" : "1");
    operand += strtol(registerToBinary(tokens[4]), NULL, 2);
   // strcat(operand, registerToBinary(tokens[4]));
    M = 1;
    opr = 8;
  } else {
    uint32_t shiftCode = numTokens > 4 ? getShiftCode(tokens[4]) : 0;
    uint32_t N;

   // strcpy(operand, numTokens > 4 ? stringToBinary(tokens[5], 6) : "000000");
    M = 0;
    if (strcmp("and", opcode) == 0) {
      opc = 0;
      opr = 0;
      N = 0;
    } else if (strcmp("bic", opcode) == 0) {
      opc = 0;
      opr = 0;
      N = 1;
    } else if (strcmp("orr", opcode) == 0) {
      opc = 1;
      opr = 0;
      N = 0;
    } else if (strcmp("orn", opcode) == 0) {
      opc = 1;
      opr = 0;
      N = 1;
    } else if (strcmp("eor", opcode) == 0) {
      opc = 2;
      opr = 0;
      N = 0;
    } else if (strcmp("eon", opcode) == 0) {
      opc = 2;
      opr = 0;
      N = 1;
    } else if (strcmp("ands", opcode) == 0) {
      opc = 3;
      opr = 0;
      N = 0;
    } else if (strcmp("bics", opcode) == 0) {
      opc = 3;
      opr = 0;
      N = 1;
    } else { // arithmetic
      opc = opcArithmetic(opcode);
      opr = 1;
      N = 0;
    }

    opr += shiftCode << 1;
    opr += N;

   // strcat(opr, shiftCode);
   // strcat(opr, N);
  }

  uint32_t res = sf << 31;
  res += opc << 29;
  res += M << 28;
  res += 0x5 << 25;
  res += opr << 21;
  res += rm << 16;
  res += operand << 10;
  res += rn << 5;
  res += rd;

  //strcpy(res, sf);
  //strcat(res, opc);
  //strcat(res, M);
  //strcat(res, "101");
  //strcat(res, opr);
  //strcat(res, rm);
  //strcat(res, operand);
  //strcat(res, rn);
  //strcat(res, rd);
  //strcat(res, "\0");

  //if (strcmp("madd", opcode) == 0 || strcmp("msub", opcode) == 0 || numTokens > 4) {
  //free(operand);
  //}
 // free(opr);


  return res;
}

uint32_t DP(char** tokens, int numTokens) {
  if (strcmp(tokens[0], "movn") == 0 || strcmp(tokens[0], "movk") == 0 || strcmp(tokens[0], "movz") == 0 ||!(isRegister(tokens[3]))) {
    return DPImm(tokens, numTokens);
  }
  return DPReg(tokens, numTokens);
}
