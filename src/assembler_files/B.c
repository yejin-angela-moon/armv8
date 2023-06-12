#include "B.h"

static uint32_t conditional(row *table, char **token, int cond, uint32_t *currAddress) {
  uint32_t instruction = 0x54000000;
  instruction += cond;
  // add the address of simm19 << 5
  uint32_t address;
  if (strcmp(token[1], "0") != 0 && strtol(token[1], NULL, 16) == 0){
    address = findAddressTable(token[1], table);
  } else {
    address = strtol(token[1], NULL, 16);
  }
  int32_t offset = address - *currAddress;
  instruction += (offset & 0x7FFFF) << 5;
  return instruction;
}

uint32_t B(row *table, char **token, uint32_t *currAddress) {
  uint32_t instruction = 0;
  if (strcmp(token[0], "b") == 0) {
    instruction += 0x14000000;
    // add the address of simm26
    uint32_t address;
    if (strcmp(token[1], "0") != 0 && strtol(token[1], NULL, 16) == 0){
      address = findAddressTable(token[1], table);
    } else {
      address = strtol(token[1], NULL, 16);
    }
    int32_t offset = address - *currAddress;
    instruction += offset & 0x3FFFFFF;
  } else if (strcmp(token[0], "br") == 0) {
    instruction += 0xD61F0000;
    // add the register << 5
    uint32_t reg = registerToBinary(token[1]);
    instruction += reg << 5;
  } else {
    char *condStr = (char *) malloc(sizeof(char) * strlen(token[0]));
    strcpy(condStr, token[0]);
    //strncpy(condStr, 2, 2);
    if (strcmp(condStr, "EQ") == 0) {
      free(condStr);
      return conditional(table, token, 0, currAddress);
    } else if (strcmp(condStr, "NE") == 0) {
      free(condStr);
      return conditional(table, token, 1, currAddress);
    } else if (strcmp(condStr, "GE") == 0) {
      free(condStr);
      return conditional(table, token, 0xA, currAddress);
    } else if (strcmp(condStr, "LT") == 0) {
      free(condStr);
      return conditional(table, token, 0xB, currAddress);
    } else if (strcmp(condStr, "GT") == 0) {
      free(condStr);
      return conditional(table, token, 0xC, currAddress);
    } else if (strcmp(condStr, "LE") == 0) {
      free(condStr);
      return conditional(table, token, 0xD, currAddress);
    } else if (strcmp(condStr, "AL") == 0) {
      free(condStr);
      return conditional(table, token, 0xE, currAddress);
    }
  }
  return instruction;
}
