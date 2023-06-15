#include "B.h"
#include "utilities.h"

static uint32_t conditional(symbol_table_row *table, char **token, int cond, uint32_t currAddress) {
  uint32_t instruction = CONDITIONAL_INSTRUCTION; // binary instruction 01010100000000000000000000000000
  instruction += cond;
  // add the address of simm19 << 5

  int32_t address;
  if ((strcmp(token[1], "0x0") != 0 || strcmp(token[1], "0") != 0) && strtol(token[1], NULL, 0) == 0) {
    address = (int) findAddressOfLabel(token[1], table);
  } else {
    address = (int) strtol(token[1], NULL, 16);
  }

  int32_t offset = (int) (address - currAddress) / 4;
  instruction += (offset & MASK_19_BIT) << 5; // 01111111111111111111
  return instruction;
}

uint32_t B(symbol_table_row *table, char **token, uint32_t currAddress) {
  uint32_t instruction = 0;

  if (strcmp(token[0], "b") == 0) {
    instruction += B_INCREMENT; // 00010100000000000000000000000000

    uint32_t address;

    if ((strcmp(token[1], "0x0") != 0 || strcmp(token[1], "0") != 0) && strtol(token[1], NULL, 0) == 0) {
      address = findAddressOfLabel(token[1], table);
    } else {
      address = strtol(token[1], NULL, 16); // string to hex
    }
    int32_t offset = (int) (address - currAddress) / 4;
    instruction += offset & MASK_26_BIT; // 0011111111111111111111111111

  } else if (strcmp(token[0], "br") == 0) {
    instruction += BR_INCREMENT; // 11010110000111110000000000000000
    // add the register << 5
    uint32_t reg = registerToBinary(token[1]);
    instruction += reg << 5;

  } else {
    char *condStr = strtok(token[0], "b.");
    if (strcmp(condStr, "eq") == 0) {
      return conditional(table, token, 0, currAddress);
    } else if (strcmp(condStr, "ne") == 0) {
      return conditional(table, token, 1, currAddress);
    } else if (strcmp(condStr, "ge") == 0) {
      return conditional(table, token, 0xA, currAddress);
    } else if (strcmp(condStr, "lt") == 0) {
      return conditional(table, token, 0xB, currAddress);
    } else if (strcmp(condStr, "gt") == 0) {
      return conditional(table, token, 0xC, currAddress);
    } else if (strcmp(condStr, "le") == 0) {
      return conditional(table, token, 0xD, currAddress);
    } else if (strcmp(condStr, "al") == 0) {
      return conditional(table, token, 0xE, currAddress);
    }
  }

  return instruction;
}
