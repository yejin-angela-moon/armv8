#include "DPImm.h"

static void arithmetic_immediate(uint8_t sf, uint8_t opc, uint32_t operand, uint8_t Rd, state *state)  {
    uint64_t *generalRegisters = state->generalRegisters;
    Pstate pstate = state->pstate;
    bool sh = (operand >> 17) & 0x01;     // extract bit 22
    uint16_t imm12 = (operand >> 5) & 0x0FFF; // extract bits 21-10
    uint8_t rn = operand & 0x1F;             // extract bits 9-5
    uint64_t result = readRegister(rn, sf, generalRegisters);

    if (sh) {
        imm12 <<= 12;
    }

    // Operations performed depending on opc
    switch (opc) {
        case 0x0:
            //add
            result += imm12;
            break;
        case 0x1:
            //adds
            result += imm12;
            update_pstate(result, rn, imm12, 0, &pstate);
            break;
        case 0x2:
            //sub
            result -= imm12;
            break;
        case 0x3:
            //subs
            result -= imm12;
            update_pstate(result, rn, imm12, 1, &pstate);
        default:
            printf("Invalid opcode for arithmetic_immediate: %02X\n", opc);

    }
    // Store result in the destination register
    writeRegister(Rd, result, sf, generalRegisters);
}

static void wide_move_immediate(uint8_t sf, uint8_t opc, uint32_t operand, uint8_t Rd, state *state) {
    uint64_t *generalRegisters = state->generalRegisters;
    unsigned int hw = (operand >> 17) & 0x3;
    uint16_t imm16 = operand & 0xFFFF;

    uint64_t op = ((uint64_t)imm16) << (hw * 16); // calculate op

    switch (opc) {
        case 0x0: // movn
            writeRegister(Rd, ~op, sf, generalRegisters);
            break;
        case 0x2: // movz
            writeRegister(Rd, op, sf, generalRegisters);
            break;
        case 0x3: // movk
        {
            uint64_t mask = ~(0xFFFF << (hw * 16)) ;
            uint64_t value = readRegister(Rd, sf, generalRegisters);
            value &= mask;
            if (sf == 1){
                uint64_t shiftedImm = imm16 << (hw * 16);
                value |= shiftedImm;
            }
            writeRegister(Rd, value, sf, generalRegisters);
            break;
        }
        default:
            printf("Invalid opcode for wide_move_immediate: %02X\n", opc);
            exit(1);
    }
}

void DPImm(uint32_t instruction, state *state) { // data processing instruction (immediate)
    uint8_t sf  = extractBits(instruction, 31, 31);        // extract bit 31
    uint8_t opc = extractBits(instruction, 29, 30);       // extract bits 30-29
    uint8_t opi = extractBits(instruction, 24, 25);       // extract bits 25-24
    uint32_t operand = extractBits(instruction, 5, 22); // extract bits 22-5
    uint8_t rd = extractBits(instruction, 0, 4);                // extract bits 4-0

    if (opi == 0x2) {
        arithmetic_immediate(sf, opc, operand, rd, state);
    } else if (opi == 0x5) {
        wide_move_immediate(sf, opc, operand, rd, state);
    }
}
