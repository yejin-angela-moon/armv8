#include "DPReg.h"

static void arithmeticDPReg(uint8_t opc, uint8_t opr, uint8_t rd, uint8_t rn, uint8_t rm, uint8_t operand, bool sf, state *state) {
    uint64_t *generalRegisters = state->generalRegisters;

    uint8_t shift = extractBits(opr, 1, 2);
    uint64_t op2 = bitShift(shift, readRegister(rm, sf, generalRegisters), operand, sf);
    uint64_t rn_val = readRegister(rn, sf, generalRegisters);

    uint64_t valueToWrite;
    switch (opc) {
        case 0: {
            valueToWrite = rn_val + op2;
            break;
        }
        case 1: {
            valueToWrite = rn_val + op2;
            update_pstate(valueToWrite,  rn_val, op2, 0, &state->pstate);
            break;
        }
        case 2: {
            valueToWrite = rn_val - op2;
            break;
        }
        case 3: {
            valueToWrite = rn_val - op2;
            update_pstate(valueToWrite, rn_val, op2, 1, &state->pstate);
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
    
    if (opc == 0) {
        if (n) {
            valueToWrite = rn_val & ~op2;
        } else {
            valueToWrite = rn_val & op2;
        }
    } else if (opc == 1) {
        if (n) {
            valueToWrite = rn_val | ~op2;
        } else {
            valueToWrite = rn_val | op2;
        }
    } else if (opc == 2) {
        if (n) {
            valueToWrite = rn_val ^ ~op2;
        } else {
            valueToWrite = rn_val ^ op2;
        }
    } else if (opc == 3) {
        if (n) {
            valueToWrite = rn_val & ~op2;
        } else {
            valueToWrite = rn_val & op2;
        }
        state->pstate.N = extractBits(valueToWrite, 63, 63);
        state->pstate.Z = valueToWrite == 0;
        state->pstate.C = 0;
        state->pstate.V = 0;
    }

    writeRegister(rd, valueToWrite, sf, generalRegisters);
}

static void multiplyDPReg(uint8_t rd, uint8_t rn, uint8_t rm, uint8_t operand, bool sf, state *state) {
    uint64_t *generalRegisters = state->generalRegisters;
    bool x = extractBits(operand, 5, 5);
    uint8_t ra = extractBits(operand, 0, 4);
    uint64_t valueToWrite;

    uint64_t ra_val = readRegister(ra, sf, generalRegisters);
    uint64_t rn_val = readRegister(rn, sf, generalRegisters);
    uint64_t rm_val = readRegister(rm, sf, generalRegisters);

    if (x) {
        valueToWrite = ra_val + rn_val * rm_val;
    } else {
        valueToWrite = ra_val - rn_val * rm_val;
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
