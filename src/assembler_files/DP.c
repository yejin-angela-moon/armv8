#include "DP.h"

// returns the binary opc value for each arithmetic instruction as defined
static uint32_t opcArithmetic(const char *opcode) {
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

// returns the binary opc value for each wide move instruction as defined
static uint32_t opcWideMove(const char *opcode) {
  if (strcmp("movk", opcode) == 0) {
    return 3;
  } else if (strcmp("movn", opcode) == 0) {
    return 0;
  } else if (strcmp("movz", opcode) == 0) {
    return 2;
  } else {
    fprintf(stderr, "invalid opcode\n");
    exit(1);
  }
}

// returns the binary code for each shift type as defined
static uint32_t getShiftCode(char *shift) {
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

// returns the 32-bit little endian binary for a data processing instruction (immediate)
static uint32_t DPImm(char **tokens, int numTokens) {
  char *opcode = tokens[0];
  uint32_t sf = getSF(tokens[1]);
  uint32_t opi;
  uint32_t opc;
  uint32_t operand = 0;
  uint32_t rd = registerToBinary(tokens[1]);

  if (strcmp("movk", opcode) == 0 || strcmp("movn", opcode) == 0 || strcmp("movz", opcode) == 0) { // wide move
    opi = 5;
    opc = opcWideMove(opcode);
    uint32_t hw = numTokens == 3 ? 0 : strtol(tokens[4], NULL, 0) / 16;
    uint32_t imm16 = strtol(tokens[2], NULL, 0);

    operand += hw << 16;
    operand += imm16;

  } else { // arithmetic
    opi = 2;
    opc = opcArithmetic(opcode);

    uint32_t sh = numTokens == 4 || strcmp(tokens[5], "0") == 0 ? 0 : 1;

    uint16_t imm12 = strtol(tokens[3], NULL, 0);
    uint32_t rn = registerToBinary(tokens[2]);

    operand += sh << 17;
    operand += imm12 << 5;
    operand += rn;
  }

  // result is a 32-bit little-endian binary with sf as the LSB
  uint32_t res = sf << 31;
  res += opc << 29;
  res += 1 << 28;
  res += opi << 23;
  res += operand << 5;
  res += rd;

  return res;
}

// returns the 32-bit little endian binary for a data processing instruction (register)
static uint32_t DPReg(char **tokens, int numTokens) {
  char *opcode = tokens[0];
  uint32_t sf = getSF(tokens[1]);
  uint32_t opc;
  uint32_t M;
  uint32_t opr;
  uint32_t rm = registerToBinary(tokens[3]);
  uint32_t rn = registerToBinary(tokens[2]);
  uint32_t rd = registerToBinary(tokens[1]);
  uint32_t operand = 0;

  // identifies multiply instructions
  if (strcmp(opcode, "madd") == 0 || strcmp(opcode, "msub") == 0) {
    opc = 0;
    operand += (strcmp(opcode, "madd") == 0 ? 0 : 1) << 5;
    operand += registerToBinary(tokens[4]);
    M = 1;
    opr = 8;
  } else { // binary logical instructions
    uint32_t shiftCode = numTokens > 4 ? getShiftCode(tokens[4]) : 0;
    uint32_t N;
    operand += numTokens > 4 ? strtol(tokens[5], NULL, 0) : 0;
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
      printf("arith\n");
      opc = opcArithmetic(opcode);
      opr = 1 << 3;
      N = 0;
    }
    opr += shiftCode << 1;
    opr += N;
  }

  // result is a 32-bit little-endian binary with sf as the LSB
  uint32_t res = sf << 31;
  res += opc << 29;
  res += M << 28;
  res += 0x5 << 25;
  res += opr << 21;
  res += rm << 16;
  res += operand << 10;
  res += rn << 5;
  res += rd;

  return res;
}

uint32_t DP(char **tokens, int numTokens) {
  // the instruction is processing immediate value if the instruction is wide move, or the 4th token is not a register
  if (strcmp(tokens[0], "movn") == 0 || strcmp(tokens[0], "movk") == 0 || strcmp(tokens[0], "movz") == 0 ||
      !(isRegister(tokens[3]))) {
    return DPImm(tokens, numTokens);
  }
  // the instruction is processing register otherwise
  return DPReg(tokens, numTokens);
}
