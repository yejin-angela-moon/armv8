#include "DPReg.h"

static void arithmeticDPReg(uint8_t opc, uint8_t opr, uint8_t rd, uint8_t rn, uint8_t rm, uint8_t operand, bool sf, state *state) {
    uint64_t *generalRegisters = state->generalRegisters;

    uint8_t shift = extractBits(opr, 1, 2);
    uint64_t op2 = bitShift(shift, readRegister(rm, sf, generalRegisters), sf? operand: operand & 0xFFFF, sf);
    uint64_t registerN = readRegister(rn, sf, generalRegisters);

    uint64_t valueToWrite;
    switch (opc) {
        case 0: {
            valueToWrite = registerN + op2;
            break;
        }
        case 1: {
            valueToWrite = registerN + op2;
            update_pstate(valueToWrite,  registerN, op2, 0, sf, state);
            break;
        }
        case 2: {
            valueToWrite = registerN - op2;
            break;
        }
        case 3: {
            valueToWrite = registerN - op2;
            update_pstate(valueToWrite, registerN, op2, 1, sf, state);
            break;
        }
        default: ;
    }
    writeRegister(rd, valueToWrite, sf, generalRegisters);
}

static void logicalDPReg(uint8_t opc, uint8_t opr, uint8_t rd, uint8_t rn, uint8_t rm, uint8_t operand, bool sf, state *state) {
    uint64_t *generalRegisters = state->generalRegisters;

      uint8_t shift = extractBits(opr, 1, 2);
      uint64_t op2 = bitShift(shift, readRegister(rm, sf, generalRegisters), operand, sf);
      bool n = opr % 2;

      uint64_t valueToWrite;
      uint64_t rn_val = readRegister(rn, sf, generalRegisters);

      switch (opc) {
        case 0:
          valueToWrite = rn_val & (n ? ~op2 : op2);
          break;
        case 1:
          valueToWrite = rn_val | (n ? ~op2 : op2);
          break;
        case 2:
          valueToWrite = rn_val ^ (n ? ~op2 : op2);
          break;
        case 3:
          valueToWrite = rn_val & (n ? ~op2 : op2);
          state->pstate.N = extractBits(valueToWrite, 63, 63);
          state->pstate.Z = valueToWrite == 0;
          state->pstate.C = 0;
          state->pstate.V = 0;
          break;
      }

      writeRegister(rd, valueToWrite, sf, generalRegisters);

}

static void multiplyDPReg(uint8_t rd, uint8_t rn, uint8_t rm, uint8_t operand, bool sf, state *state) {
    uint64_t *generalRegisters = state->generalRegisters;
    bool x = extractBits(operand, 5, 5);
    uint8_t ra = extractBits(operand, 0, 4);
    uint64_t valueToWrite;

    if (x) {
        valueToWrite = readRegister(ra, sf, generalRegisters)
                       + readRegister(rn, sf, generalRegisters)
                         * readRegister(rm, sf, generalRegisters);
    } else {
        valueToWrite = readRegister(ra, sf, generalRegisters)
                       - readRegister(rn, sf, generalRegisters)
                         * readRegister(rm, sf, generalRegisters);
    }

    writeRegister(rd,valueToWrite, sf, generalRegisters);
}

void DPReg(uint32_t instruction, state *state) {
    uint8_t rd = extractBits(instruction, 0, 4);
    uint8_t rn = extractBits(instruction, 5, 9);
    uint8_t rm = extractBits(instruction, 16, 20);
    uint8_t operand = extractBits(instruction, 10, 15);
    bool sf = extractBits(instruction, 31, 31);
    uint8_t opc = extractBits(instruction, 29, 30);
    bool m = extractBits(instruction, 28, 28);
    uint8_t opr = extractBits(instruction, 21, 24);

    if (m == 1) {
        multiplyDPReg(rd, rn, rm, operand, sf, state);
    } else {
        if (extractBits(opr, 3, 3)) {
            arithmeticDPReg(opc, opr, rd, rn, rm, operand, sf, state);
        } else {
            logicalDPReg(opc, opr, rd, rn, rm, operand, sf, state);
        }
    }
}
