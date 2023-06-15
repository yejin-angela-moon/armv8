#include "SDT.h"
#include "definition.h"

static uint32_t preIndexed(char **tokens) {

  uint32_t instruction = 0;
  if (tokens[1][0] == 'x') {
    instruction |= 1 << 30;
  }

  int rt = getSubstringAsInt(tokens[1], 1, 2);
  instruction |= rt;

  instruction |= SDT_INSTRUCTION;

  if (strcmp(tokens[0], "ldr") == 0) {
    //ldr
    instruction |= 1 << 22;
  }
  int xn = getSubstringAsInt(tokens[2], 2, 2);
  instruction |= xn << 5;

  instruction |= 1 << 10;
  instruction |= 1 << 11;

  int32_t simm = (int) strtol(tokens[3], NULL, 0);
  if (strchr(tokens[3], '-') != NULL) {
    simm = (~simm) + 1;
  }

  instruction |= simm << 12;
  return instruction;
}

static uint32_t postIndexed(char **tokens) {
  uint32_t instruction = 0;
  if (tokens[1][0] == 'x') {
    instruction |= 1 << 30;
  }

  int rt = getSubstringAsInt(tokens[1], 1, 2);
  instruction |= rt;

  instruction |= SDT_INSTRUCTION;


  if (strcmp(tokens[0], "ldr") == 0) {
    instruction |= 1 << 22;
  }

  int xn = getSubstringAsInt(tokens[2], 2, 2);
  instruction |= xn << 5;
  instruction |= 1 << 10;

  int16_t simm;
  simm = (int16_t) strtol(strtok(tokens[3], "-"), NULL, 10);
  if (strchr(tokens[3], '-') != NULL) {
    simm = (int16_t) ((~simm) + 1);
  }

  instruction |= (simm & 0x01FF) << 12;
  return instruction;
}

static uint32_t registerOffset(char **tokens) {

  uint32_t instruction = 0;
  if (tolower(tokens[1][0]) == 'x') {
    instruction |= 1 << 30;
  }

  int rt = getSubstringAsInt(tokens[1], 1, 2);
  instruction |= rt;
  instruction |= SDT_INSTRUCTION;


  if (strcmp(tokens[0], "ldr") == 0) {
    instruction |= 1 << 22;
  }
  int xn = getSubstringAsInt(tokens[2], 2, 2);
  instruction |= xn << 5;

  instruction |= REGISTER_OFFSET << 10;

  int xm = getSubstringAsInt(tokens[3], 1, 2);
  instruction |= xm << 16;
  return instruction;
}

static uint32_t unsignedOffset(char **tokens, int countToken) {
  uint32_t instruction = 0;
  if (tokens[1][0] == 'x') {
    instruction |= 1 << 30;
  }

  int rt = getSubstringAsInt(tokens[1], 1, 2);
  instruction |= rt;

  instruction |= SDT_INSTRUCTION;


  if (strcmp(tokens[0], "ldr") == 0) {
    instruction |= 1 << 22;
  }
  int xn = getSubstringAsInt(tokens[2], 2, 2);
  instruction |= xn << 5;


  instruction |= 1 << 24;

  if (countToken > 3) {
    if (tolower(tokens[1][0]) == 'x') {
      instruction |= strtol(tokens[3], NULL, 0) / 8 << 10;
    } else {
      instruction |= strtol(tokens[3], NULL, 0) / 4 << 10;
    }
  }

  return instruction;
}

static uint32_t LL(char **tokens, symbol_table_row *symbol_table, uint32_t currAddress) {
  uint32_t instruction = 0;
  if (tolower(tokens[1][0]) == 'x') {
    instruction |= 1 << 30;
  }

  int rt = getSubstringAsInt(tokens[1], 1, 2);
  instruction |= rt;

  instruction |= LL_INSTRUCTION;

  uint32_t addressLabel = findAddressOfLabel(tokens[2], symbol_table);

  int32_t offset = (int) (addressLabel - currAddress) / 4;
  instruction |= (offset & MASK_TO_19BITS) << 5;

  return instruction;
}

static uint32_t mode(char **tokens, int countToken) {
  if (countToken >= 4 && strchr(tokens[3], '!') != NULL) {
    return preIndexed(tokens);
  } else if (countToken >= 4 && strchr(tokens[2], ']') != NULL) {
    return postIndexed(tokens);
  } else if (countToken >= 4 && isRegister(strtok(tokens[3], "]"))) {
    return registerOffset(tokens);
  } else {
    return unsignedOffset(tokens, countToken);
  }
}

uint32_t SDT(char **tokens, symbol_table_row *symbol_table, int countToken, uint32_t currAddress) {
  if (strchr(tokens[2], '[') == NULL) {
    return LL(tokens, symbol_table, currAddress);
  }
  return mode(tokens, countToken);
}
