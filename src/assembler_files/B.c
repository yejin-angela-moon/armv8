#include "B.h"

static uint_64 conditional(char **token, int cond) {
  instruction += 0x54000000;
  instruction += cond;
  // add the address of simm19 << 5
  return instruction
}

uint_64 B(row *table, char **token) {
  uint_64 instruction = 0;
  if (token[0] == "b") {
    instruction += 0x14000000;
    // add the address of simm26
    instruction += table
    return instruction;
  } else if (token[0] == "br") {
    instruction += 0xD61F0000;
    // add the address of the register << 5
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
