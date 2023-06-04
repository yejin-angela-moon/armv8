#include "DPImm.h"

static void arithmetic_immediate(bool sf, uint8_t opc, uint32_t operand, uint8_t Rd, state *state)  {
    uint64_t *generalRegisters = state->generalRegisters;
    //Pstate pstate = state->pstate;

    bool sh = extractBits(operand, 17, 17);
    uint64_t imm12 = extractBits(operand, 5, 16);
    uint8_t rn = extractBits(operand, 0, 4);
    uint64_t result = readRegister(rn, sf, generalRegisters);
    uint64_t resultCopy = readRegister(rn, sf, generalRegisters);

    if (sh) {
        imm12 <<= 0xC;
    }

    switch (opc) {
        case 0: //add
            result += imm12;
            break;
        case 1:
            //adds
            result += imm12;
            update_pstate(result, resultCopy, imm12, 0, sf, state);
            break;
        case 2: //sub
            result -= imm12;
            break;
        case 3: //subs
            result -= imm12;
            resultCopy = result;
            //if ()
            update_pstate(resultCopy, resultCopy, imm12, 1, sf, state);
            break;
        default: printf("invalid opc\n");;
    }
    // Store result in the destination register
    writeRegister(Rd, result, sf, generalRegisters);
}

static void wide_move_immediate(bool sf, uint8_t opc, uint32_t operand, uint8_t Rd, state *state) {
    uint64_t *generalRegisters = state->generalRegisters;

    uint8_t hw = extractBits(operand, 16, 17);
    uint64_t imm16 = extractBits(operand, 0, 15);
    uint64_t shift = hw * 16;
    uint64_t op = imm16 << shift;

    switch (opc) {
        case 0: // movn
            writeRegister(Rd, ~op, sf, generalRegisters);
            break;
        case 2: // movz
            writeRegister(Rd, op, sf, generalRegisters);
            break;
        case 3: { // movk
            uint64_t movk_val = readRegister(Rd, sf, generalRegisters);
            movk_val &= ~(0xFFFFULL << shift);
            movk_val |= op;
            writeRegister(Rd, movk_val, sf, generalRegisters);
            break;
        }
        default: printf("invalid opc\n");
    }
}

void DPImm(uint32_t instruction, state *state) {
    uint8_t sf  = extractBits(instruction, 31, 31);
    uint8_t opc = extractBits(instruction, 29, 30);
    uint8_t opi = extractBits(instruction, 23, 25);
    uint32_t operand = extractBits(instruction, 5, 22);
    uint8_t Rd = extractBits(instruction, 0, 4);

    if (opi == 2) {
        arithmetic_immediate(sf, opc, operand, Rd, state);
    } else if (opi == 5) {
        wide_move_immediate(sf, opc, operand, Rd, state);
    }
}
