#include "SDT.h"

static uint32_t preIndexed(char **token, uint32_t instruction) {
  instruction |= 1 << 10;
  instruction |= 1 << 11;

  int32_t simm = stringToNumber(token[3]);
  instruction |= (simm & MASK_9) << 12;

  return instruction;
}

static uint32_t postIndexed(char **token, uint32_t instruction, int countToken) {
  instruction |= 1 << 10;

  int16_t simm = stringToNumber(token[3]);

  instruction |= (simm & MASK_9) << 12;
  return instruction;
}

static uint32_t registerOffset(char **token, uint32_t instruction) {
  instruction |= REGISTER_OFFSET << 10;

  int xm = stringToNumber(token[3]);
  instruction |= xm << 16;
  return instruction;
}

static uint32_t unsignedOffset(char **token, uint32_t instruction, int countToken) {
  instruction |= 1 << 24;

  if (countToken > 3) {
    if (token[1][0] == 'x') {
      instruction |= stringToNumber(token[3]) / 8 << 10;
    } else {
      instruction |= stringToNumber(token[3]) / 4 << 10;
    }
  }

  return instruction;
}

static uint32_t LL(char **token, row *table, uint32_t instruction, uint32_t currAddress) {
  instruction |= LL_INSTRUC;

  uint32_t addressLabel = findAddressTable(token[2], table);

  int32_t offset = (addressLabel - currAddress) / 4;
  instruction |= (offset & MASK_TO_19BITS) << 5;

  return instruction;
}

uint32_t mode(char **token, uint32_t instruction, int countToken) {

  int xn = getNum(token[2], 2, 2);
  instruction |= xn << 5;

  if (countToken >= 4 && strchr(token[3], '!') != NULL) {
    return preIndexed(token, instruction);
  } else if (countToken >= 4 && strchr(token[2], ']') != NULL) {
    return postIndexed(token, instruction, countToken);
  } else if (countToken >= 4 && isRegister(token[3])) {
    return registerOffset(token, instruction);
  } else {
    return unsignedOffset(token, instruction, countToken);
  }
}

uint32_t SDT(char **token, row *table, int countToken, uint32_t currAddress) {

  uint32_t instruction = 0;
  if (token[1][0] == 'x') {
    instruction |= 1 << 30;
  }

  int rt = getNum(token[1], 1, 2);
  instruction |= rt;

  if (strchr(token[2], '[') == NULL) {
    return LL(token, table, instruction, currAddress);
  }


  instruction |= SDT_INSTRUC;


  if (strcmp(token[0], "ldr") == 0) {
    //ldr
    instruction |= 1 << 22;
  }
  return mode(token, instruction, countToken);
}

