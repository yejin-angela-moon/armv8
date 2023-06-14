#include "B.h"
#include "utilities.h"

static uint32_t conditional(row *table, char **token, int cond, uint32_t currAddress) {
  uint32_t instruction = 0x54000000;
  instruction += cond;
  int32_t address;
  if (!isdigit(token[1][0])) {
    address = findAddressTable(token[1], table);
  } else {
    address = strtol(token[1], NULL, 16);
  }

  int32_t offset = (address - currAddress) / 4;
  instruction += (offset & 0x7FFFF) << 5;
  return instruction;
}

uint32_t B(row *table, char **token, uint32_t currAddress) {
  uint32_t instruction = 0;
  if (strcmp(token[0], "b") == 0) {
    instruction += 0x14000000;
    uint32_t address;

    if (!isdigit(token[1][0])) {
      address = findAddressTable(token[1], table);
    } else {
      address = stringToNumber(token[1]);
    }
    int32_t offset = (address - currAddress) / 4;
    instruction += offset & 0x3FFFFFF;
  } else if (strcmp(token[0], "br") == 0) {
    instruction += 0xD61F0000;
    uint32_t reg = stringToNumber(token[1] + 1);
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
