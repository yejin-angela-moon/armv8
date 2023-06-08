#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "DP.h"

#define DP_MUL_NUM 2;
#define DP_TWO_OPERAND_NUM 12;
#define DP_SINGLE_OPERAND NUM 9;
#define DP_TWO_OPERAND_NODEST_NUM 3;
#define DP_MAX_TOKENS 4;

typedef void (*FunctionPointer)(const char *args);

typedef struct {
  const char* string;
  FunctionPointer function;
} StringFunctionMapping;

uint32_t arithmetic(char* tokens) {
  uint8_t rd = (uint8_t) registerToNumber(tokens[1]);

  char rn[] = tokens[2];
  char operand[] = tokens[3];

  // maybe strings representing binary e.g. "101010" and concatenate the strings
  // and then convert into binary using strtol?
  
}

/*const char* mulSet[] = {"madd", "msub"}; // Multiply: <mul_opcode> rd, rn, rm, ra
const char* twoOperandSet[] = {"add", "adds", "sub", "subs", "and", "ands", "bic", "bics", "eor", "orr", "eon", "orn"}; // Two operand: <opcode> rd, rn, <operand>. Arithmetic and bit-logic operations
const char* singleOperandSet[] = {"mov", "mul", "mneg", "neg", "negs", "mvn", "movz", "movn", "movk"}; // Single operand with destination: <opcode> rd, <operand>
const char* twoOperandsNoDestSet[] = {"cmp", "cmn", "tst"}; // Two operands, no destination: <opcode> rn, <operand>  */

uint32_t DP(char* tokens, int numTokens) {
  char opcode = tokens[0];

  StringFunctionMapping dpMappings[] = {
    {"add", add}, {"adds", add}
    {"sub", sub}, {"subs", sub}
  }

  //if (isStringInSet(tokens[0], mulSet, DP_MUL_NUM)) {
    
 // }
 
  for (int i = 0; i < sizeof(dpMappings) / sizeof(dpMappings[0]); i++) {
    if (strcmp(tokens, dpMappings[i]->string) == 0) {
      dpMappings[i]->function(tokens);
      return;
    }
  }

 /* if (numTokens == DP_MAX_TOKENS && isRegister(tokens[numTokens])) {
    multiply(tokens);
  } else if () {
    return;
  }*/

  uint32_t output = switch (tokens[0]) {
    case "add":
    case "adds":
    case "sub":
    case "subs":
      arithmetic(tokens);
      break;
    case "movk":
    case "movn":
    case "movz":
      wide_move(tokens);
    default: printf(stderr, "invalid opcode\n");
  }
