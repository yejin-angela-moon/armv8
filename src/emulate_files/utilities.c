#include "utilities.h"

state *initialise(void) {
  state *newState = malloc(sizeof(state)); // allocate memory for state
  newState->currAddress = 0;
  memset((*newState).generalRegisters, 0, sizeof((*newState).generalRegisters));
  newState->pstate.C = 0;
  newState->pstate.V = 0;
  newState->pstate.N = 0;
  newState->pstate.Z = 1;
  newState->memory = (uint32_t *) calloc(MEMORY_SIZE, sizeof(uint32_t));
  return newState;
}

void inc_PC(state *state) {
  state->currAddress += 4; // PC increments by 4 bits each time
}

void writeRegister(uint8_t registerIndex, uint64_t newValue, bool sf, uint64_t *generalRegisters) {
  assert(registerIndex >= 0 && registerIndex <= NUM_REGISTERS);

  if (registerIndex == NUM_REGISTERS) { // ZR
    return;
  }

  if (sf) {
    // Write to X-register: use the whole 64-bit value
    generalRegisters[registerIndex] = newValue;
  } else {
    // Write to W-register: zero out the upper 32 bits
    generalRegisters[registerIndex] = newValue & LOWER_32_BITS;
  }
}

uint64_t readRegister(uint8_t registerIndex, bool sf, uint64_t *generalRegisters) {
  assert(registerIndex >= 0 && registerIndex <= NUM_REGISTERS);

  if (registerIndex == NUM_REGISTERS) { // ZR
    return 0;
  }

  if (sf) {
    // Read from X-register: return the whole 64-bit value
    return generalRegisters[registerIndex];
  } else {
    // Read from W-register: return only the lower 32 bits
    return generalRegisters[registerIndex] & LOWER_32_BITS;
  }
}

uint32_t extractBits(uint64_t n, uint8_t startIndex, uint8_t endIndex) {
  // start/endIndex is inclusive, right-to-left starting from 0
  uint32_t mask = (1ULL << (endIndex - startIndex + 1)) - 1;
  return (n >> startIndex) & mask;
}

void update_pstate(uint64_t result, uint64_t operand1, uint64_t operand2, bool is_subtraction, bool sf, state *state) {

  //state->pstate.N = 0;
  // Update N and Z flags
  if (sf) {
    state->pstate.N = result & (1ULL << MSB_64);  // check the most significant bit
    if (is_subtraction) {
      // For subtraction, carry is set if operand1 >= operand2
      state->pstate.C = operand1 >= operand2;

      // Overflow for subtraction is set if operand1 and operand2 have different signs,
      // and operand1 and the result have different signs
      state->pstate.V = ((operand1 ^ operand2) & (operand1 ^ result)) >> MSB_64;
    } else {
      // For addition, carry is set if result is less than either operand (meaning it wrapped around)
      state->pstate.C = result < operand1 || result < operand2;

      // Overflow for addition is set if operand1 and operand2 have the same sign,
      // and operand1 and the result have different signs
      state->pstate.V = (~(operand1 ^ operand2) & (operand1 ^ result)) >> MSB_64;
    }
  } else {
    state->pstate.N = result & (1ULL << MSB_32);  // check the most significant bit
    uint32_t op1 = (uint32_t) operand1;
    uint32_t op2 = (uint32_t) operand2;
    if (is_subtraction) {
      // For subtraction, carry is set if operand1 >= operand2
      state->pstate.C = op1 >= op2;

      // Overflow for subtraction is set if operand1 and operand2 have different signs,
      // and operand1 and the result have different signs
      state->pstate.V = ((op1 ^ op2) & (op1 ^ result)) >> MSB_64;
    } else {
      // For addition, carry is set if result is less than either operand (meaning it wrapped around)
      state->pstate.C = result < op1 || result < op2;

      // Overflow for addition is set if operand1 and operand2 have the same sign,
      // and operand1 and the result have different signs
      state->pstate.V = (~(op1 ^ operand2) & (op1 ^ result)) >> MSB_64;
    }
  }

  state->pstate.Z = (result == 0);
}

uint64_t bitShift(uint8_t shift, uint64_t n, uint8_t operand, bool sf) {

  switch (shift) {
    case 0: //lsl
      return n << operand;
    case 1: //lsr
      return n >> operand;
    case 2: //asr
      return sf ? ((int64_t) n) >> operand : ((int32_t) n) >> operand;
    case 3: { //ror
      int bitCount = sizeof(n) * 8; // Calculates the total number of bits for the data type
      operand %= bitCount; // Just in case, reduce the number of rotations to a number less than bitCount
      if (sf) {
        return (n >> operand) | (n << (bitCount - operand));
      } else {
        uint32_t smaller_n = (uint32_t) n;
        return (smaller_n >> operand) | (smaller_n << (bitCount - operand));
      }
    }
    default:
      fprintf(stderr, "invalid shift code\n");
      exit(1);
  }
}


