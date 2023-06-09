#include "B.h"

static uint_32_t conditional(char **token, int cond) {
  uint32_t instruction = 0x54000000;
  instruction += cond;
  // add the address of simm19 << 5
  uint32_t address;
  if (token[1] != 0 && atoi(token[1]) == 0){
    address = findAddressTable(token[1], table);
  } else {
    address = token[1];
  }
  int32_t offset = address - currAddress;
  instruction += (offset & 0x7FFFF) << 5
  return instruction;
}

uint_32_t B(row *table, char **token) {
  uint_32_t instruction = 0;
  if (strcmp(token[0], "b") == 0) {
    instruction += 0x14000000;
    // add the address of simm26
    uint32_t address;
      if (token[1] != 0 && atoi(token[1]) == 0){
        address = findAddressTable(token[1], table);
      } else {
        address = token[1];
      }
    int32_t offset = address - currAddress;
    instruction += offset & 0x3FFFFFF;
    return instruction;
  } else if (strcmp(token[0], "br") == 0) {
    instruction += 0xD61F0000;
    // add the register << 5
    uint32_t reg = registerToBinary(token[1]);
    instruction += reg << 5
    return instruction;
  } else {
    char condStr[] =  token[0];
    strncpy(condStr, 2, 2);
    if (strcmp(condStr, "EQ") == 0) {
      return conditional(token, 0);
    } else if (strcmp(condStr, "NE") == 0) {
      return conditional(token, 1);
    } else if (strcmp(condStr, "GE") == 0) {
      return conditional(token, 0xA);
    } else if (strcmp(condStr, "LT") == 0) {
      return conditional(token, 0xB);
    } else if (strcmp(condStr, "GT") == 0) {
      return conditional(token, 0xC);
    } else if (strcmp(condStr, "LE") == 0) {
      return conditional(token, 0xD);
    } else if (strcmp(condStr, "AL") == 0) {
      return conditional(token, 0xE);
    }
  }

}
