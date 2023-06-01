#include "B.h"

static uint64_t signExtension(unsigned int x) {
    return (uint64_t)((uint32_t)x);
}

void B(uint32_t instruction, state *state) {
    Pstate pstate = state->pstate;

    uint8_t unconditional = extractBits(instruction, 26, 31);
    uint32_t reg1 = extractBits(instruction, 10, 31);
    uint8_t reg2 = extractBits(instruction, 0, 4);
    uint8_t condition1 = extractBits(instruction, 24, 31);
    uint8_t condition2 = extractBits(instruction, 4, 4);
    if (unconditional == 0x5) {
        //Unconditional
        uint32_t simm26 = extractBits(instruction, 0, 25);
        uint64_t offset = signExtension(simm26) * 4;
        state->currAddress += offset;
    } else if (reg2 == 0x0 && reg1 == 0x3587c0) {
        //Register
        uint8_t xn = extractBits(instruction, 5, 9);
        state->currAddress = readRegister(xn, 0, state->generalRegisters);
    } else if (condition2 == 0x0 && condition1 == 0x54) {
        //Conditional
        uint32_t simm19 = extractBits(instruction, 5, 23);
        uint64_t offset = signExtension(simm19) * 4;
        uint8_t cond = extractBits(instruction, 0, 3);
        if (cond == 0x0 && pstate.Z == 1) {
            state->currAddress += offset;
        } else if (cond == 0x1 && pstate.Z == 0) {
            state->currAddress += offset;
        } else if (cond == 0x6 && pstate.N == 1) {
            state->currAddress += offset;
        } else if (cond == 0x7 && pstate.N != 1) {
            state->currAddress += offset;
        } else if (cond == 0xC && pstate.Z == 0 && pstate.N == pstate.V) {
            state->currAddress += offset;
        } else if (cond == 0xD && (pstate.Z == 0 || pstate.N == pstate.V)) {
            state->currAddress += offset;
        } else if (cond == 0xE) {
            state->currAddress += offset;
        }
    }
}
