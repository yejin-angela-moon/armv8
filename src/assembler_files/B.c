#include "B.h"
#include "utilities.h"

static uint32_t conditional(row *table, char **token, int cond, uint32_t *currAddress) {
  uint32_t instruction = 0x54000000;
  instruction += cond;
  // add the address of simm19 << 5
  int32_t address;
  //char *endptr;
//  printf("another %s", strchr(token[1], '0'));
  //char isHex[2];
  //strncpy(isHex, token[1], 2);
 // int num = getNum(token[1], 2, strlen(token[1]) - 2);
  if ((strcmp(token[1], "0x0") != 0 || strcmp(token[1], "0") != 0 ) && strtol(token[1], NULL, 0) == 0){
    address = findAddressTable(token[1], table);
    //printf("another %x from table", address);
  } else {
    address = strtol(token[1], NULL, 16);
  }

  int32_t offset = (address - *currAddress) / 4;
  instruction += (offset & 0x7FFFF) << 5;
  return instruction;
}

uint32_t B(row *table, char **token, uint32_t *currAddress) {
  uint32_t instruction = 0;
  if (strcmp(token[0], "b") == 0) {
    //printf("token = b\n");
    instruction += 0x14000000;
    //printf("mid ins %x", instruction);
    // add the address of simm26
    uint32_t address;
    //char isHex[2];
    //strncpy(isHex, token[1], 2);
   // int num = getNum(token[1], 2, strlen(token[1]) - 2);
    if ((strcmp(token[1], "0x0") != 0 || strcmp(token[1], "0") != 0 ) && strtol(token[1], NULL, 0) == 0){
      //printf("get same\n");
      address = findAddressTable(token[1], table);
     // printf("another %x from table", address);
    } else {
     // printf("get addr\n");
      address = strtol(token[1], NULL, 16);
    }
    int32_t offset = (address - *currAddress) / 4;
    instruction += offset & 0x3FFFFFF;
    //printf("get instr\n");
  } else if (strcmp(token[0], "br") == 0) {
    instruction += 0xD61F0000;
    // add the register << 5
    uint32_t reg = stringToNumber(token[1] + 1);
    instruction += reg << 5;
  } else {
    //char *condStr = (char *) malloc(sizeof(char) * strlen(token[0]));
    //strcpy(condStr, token[0]);
    char *condStr = strtok(token[0], "b.");
    //printf("%s\n", condStr);
    //strncpy(condStr, 2, 2);
    if (strcmp(condStr, "eq") == 0) {
      //free(condStr);
      return conditional(table, token, 0, currAddress);
    } else if (strcmp(condStr, "ne") == 0) {
      //free(condStr);
      return conditional(table, token, 1, currAddress);
    } else if (strcmp(condStr, "ge") == 0) {
      //free(condStr);
      return conditional(table, token, 0xA, currAddress);
    } else if (strcmp(condStr, "lt") == 0) {
      //free(condStr);
      return conditional(table, token, 0xB, currAddress);
    } else if (strcmp(condStr, "gt") == 0) {
      //free(condStr);
      return conditional(table, token, 0xC, currAddress);
    } else if (strcmp(condStr, "le") == 0) {
      //free(condStr);
      return conditional(table, token, 0xD, currAddress);
    } else if (strcmp(condStr, "al") == 0) {
      //free(condStr);
      return conditional(table, token, 0xE, currAddress);
    }
  }
  //printf("%x", instruction);
  return instruction;
}
