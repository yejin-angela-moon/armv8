#include "B.h"
#include "utilities.h"

static uint32_t conditional(symbol_table_row *symbol_table, char **tokens, int cond, uint32_t currAddress) {
  uint32_t instruction = CONDITIONAL_INSTRUCTION; // binary instruction 01010100000000000000000000000000
  instruction |= cond;
  // add the address of simm19 << 5

  int32_t address;
  if ((strcmp(tokens[1], "0x0") != 0 || strcmp(tokens[1], "0") != 0) && strtol(tokens[1], NULL, 0) == 0) {
    address = findAddressOfLabel(tokens[1], symbol_table);
  } else {
    address = strtol(tokens[1], NULL, 0);
  }

  int32_t offset = (address - currAddress) / 4;
  instruction |= (offset & MASK_19_BIT) << 5; // 01111111111111111111
  return instruction;
}

uint32_t B(symbol_table_row *table, char **tokens, uint32_t currAddress) {
  uint32_t instruction = 0;

  if (strcmp(tokens[0], "b") == 0) {
    instruction |= B_INCREMENT; // 00010100000000000000000000000000

    uint32_t address;

    if ((strcmp(tokens[1], "0x0") != 0 || strcmp(tokens[1], "0") != 0) && strtol(tokens[1], NULL, 0) == 0) {
      address = findAddressOfLabel(tokens[1], table);
    } else {
      address = strtol(tokens[1], NULL, 0); // string to hex
    }
    int32_t offset = (address - currAddress) / 4;
    instruction |= offset & MASK_26_BIT; // 0011111111111111111111111111

  } else if (strcmp(tokens[0], "br") == 0) {
    instruction |= BR_INCREMENT; // 11010110000111110000000000000000
    // add the register << 5
    uint32_t reg = registerIndexAsInt(tokens[1]);
    instruction |= reg << 5;

  } else { // the conditions that a b.cond instruction can take, and their condition code cond
    char *condStr = strtok(tokens[0], "b.");
    if (strcmp(condStr, "eq") == 0) {
      return conditional(table, tokens, 0, currAddress);
    } else if (strcmp(condStr, "ne") == 0) {
      return conditional(table, tokens, 1, currAddress);
    } else if (strcmp(condStr, "ge") == 0) {
      return conditional(table, tokens, 0xA, currAddress);
    } else if (strcmp(condStr, "lt") == 0) {
      return conditional(table, tokens, 0xB, currAddress);
    } else if (strcmp(condStr, "gt") == 0) {
      return conditional(table, tokens, 0xC, currAddress);
    } else if (strcmp(condStr, "le") == 0) {
      return conditional(table, tokens, 0xD, currAddress);
    } else if (strcmp(condStr, "al") == 0) {
      return conditional(table, tokens, 0xE, currAddress);
    }
  }

  return instruction;
}
