#include "utilities.h"

void initialise(state *state) {
    state -> currAddress = 0;
    memset((*state).generalRegisters, 0, sizeof((*state).generalRegisters));
    state -> pstate.C = 0;
    state -> pstate.V = 0;
    state -> pstate.N = 0;
    state -> pstate.Z = 1;
}

void inc_PC (state *state){
    state -> currAddress += 0x4;
}

void writeRegister (int registerIndex, uint64_t newValue, uint8_t sf, uint64_t *generalRegisters) {
    if (registerIndex == 31) {
        return;
    }
    if (sf == 0) {
        // Write to W-register: zero out the upper 32 bits
        generalRegisters[registerIndex] = newValue & 0xFFFFFFFF;
    } else {
        // Write to X-register: use the whole 64-bit value
        generalRegisters[registerIndex] = newValue;
    }
}

uint64_t readRegister (int registerIndex, uint8_t sf, uint64_t *generalRegisters) {
    if (registerIndex == 31) {
        return 0;
    }
    if (sf == 0) {
        // Read from W-register: return only the lower 32 bits
        return generalRegisters[registerIndex] & 0xFFFFFFFF;
    } else {
        // Read from X-register: return the whole 64-bit value
        return generalRegisters[registerIndex];
    }
}

uint32_t extractBits(uint64_t n, uint8_t startIndex, uint8_t endIndex) {
    // start/endIndex is inclusive, right-to-left starting from 0
    int mask = (1 << (endIndex - startIndex + 1)) - 1;
    return (n >> startIndex) & mask;
}

void update_pstate(uint64_t result, uint64_t operand1, uint64_t operand2, bool is_subtraction, Pstate *pstate) {
    // Update N and Z flags
    pstate->N = result & (1ULL << 63);  // check the most significant bit
    pstate->Z = (result == 0);

    if (is_subtraction) {
        // For subtraction, carry is set if operand1 >= operand2
        pstate->C = operand1 >= operand2;

        // Overflow for subtraction is set if operand1 and operand2 have different signs,
        // and operand1 and the result have different signs
        pstate->V = ((operand1 ^ operand2) & (operand1 ^ result)) >> 63;
    } else {
        // For addition, carry is set if result is less than either operand (meaning it wrapped around)
        pstate->C = result < operand1 || result < operand2;

        // Overflow for addition is set if operand1 and operand2 have the same sign,
        // and operand1 and the result have different signs
        pstate->V = (~(operand1 ^ operand2) & (operand1 ^ result)) >> 63;
    }
}

uint8_t bitShift(uint8_t shift, int64_t n, uint8_t operand) {
    switch (shift) {
        case 0:
            //lsl
            return n << operand;
        case 1:
            //lsr
            return n >> operand;
        case 2:
            //asr
            if (n < 0 && shift > 0) {
                return (n >> shift) | ~(~0U >> shift);
            } else {
                return n >> shift;
            }
        case 3: {
            //ror
            int bitCount = sizeof(n) * 8; // Calculates the total number of bits for the data type
            operand %= bitCount; // Just in case, reduce the number of rotations to a number less than bitCount
            return (n >> operand) | (n << (bitCount - operand));
        }
        default: return 0;
    }
}

unsigned int get_MSB(unsigned int num) {
    unsigned int MSB = 0;
    while (num != 0) {
        num = num / 2;
        MSB++;
    }
    return MSB;
}
