#include "B.h"

static void unconditional(uint32_t simm26, state *state) {
    int64_t offset = (int64_t) (simm26 - 1) * 4;
    state->currAddress += offset;
}

static void reg(uint8_t xn, state *state) {
    state->currAddress = readRegister(xn, 1, state->generalRegisters);
}

static void conditional(uint32_t simm19, uint8_t cond, state *state) {
    //Pstate pstate = state->pstate;
    int64_t offset = (int64_t) (simm19 - 1) * 4;
    if (cond == 0x0 && state->pstate.Z == 1) {
        state->currAddress += offset;
    } else if (cond == 0x1 && state->pstate.Z == 0) {
        state->currAddress += offset;
    } else if (cond == 0xA && (state->pstate.N == state->pstate.V)) {
        state->currAddress += offset;
    } else if (cond == 0xB && state->pstate.N != state->pstate.V) {
        state->currAddress += offset;
    } else if (cond == 0xC && state->pstate.Z == 0 && state->pstate.N == state->pstate.V) {
        state->currAddress += offset;
    } else if (cond == 0xD && (state->pstate.Z == 0 || state->pstate.N == state->pstate.V)) {
        state->currAddress += offset;
    } else if (cond == 0xE) {
        state->currAddress += offset;
    }
    //state->currAddress += offset;
}


void B(uint32_t instruction, state *state) {


    //uint32_t reg1 = extractBits(instruction, 10, 31);
    //uint8_t reg2 = extractBits(instruction, 0, 4);
    //uint8_t condition1 = extractBits(instruction, 24, 31);
    //uint8_t condition2 = extractBits(instruction, 4, 4);
    uint32_t simm26 = extractBits(instruction, 0, 25);
    uint8_t xn = extractBits(instruction, 5, 9);
    uint32_t simm19 = extractBits(instruction, 5, 23);
    uint8_t cond = extractBits(instruction, 0, 3);
   if (extractBits(instruction, 0, 4) == 0x0 && extractBits(instruction, 10, 31) == 0x3587c0) {
        //Register
        reg(xn, state);
    } else if (extractBits(instruction, 4, 4) == 0x0 && extractBits(instruction, 24, 31) == 0x54) {
        //conditional
        conditional(simm19, cond, state);
        //conditional(simm19, cond, state);
    } else {
       unconditional(simm26, state);

    }
}
