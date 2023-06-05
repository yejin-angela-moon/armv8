#include "B.h"
#include <math.h>

int64_t signExtension(uint32_t instr) {
    int64_t value = (0x00000000FFFFFFFF & instr);
    if (instr >> 31) {
        return -8;
        value += 0xFFFFFFFF00000000;
    }
    return value;
}

void modify(int64_t offset, state *state) {
   if (offset < 0) {
        state->currAddress -= abs(offset);
   } else {
        state->currAddress += offset;
   }
   state->currAddress -= 4;
}

static void unconditional(uint32_t simm26, state *state) {
    int64_t offset = (int64_t) (simm26 - 1) * 4;
    state->currAddress += offset;
}

static void reg(uint8_t xn, state *state) {
    state->currAddress = readRegister(xn, 1, state->generalRegisters);
}

static void conditional(uint32_t simm19, uint8_t cond, state *state) {
    //Pstate pstate = state->pstate;
    //uint32_t testing = 0xFFFFFFFF;
   // int64_t offset = signExtension(simm19);
   // uint64_t off = (uint64_t) offset;
   //int64_t value = Math.Abs(offset);
    if (cond == 0x0 && state->pstate.Z == 1) {
        //state->currAddress += offset;
        modify(signExtension(simm19), state);
    } else if (cond == 0x1 && state->pstate.Z == 0) {
        modify(signExtension(simm19), state);
    } else if (cond == 0xA && (state->pstate.N == state->pstate.V)) {
       // state->currAddress += offset;
        modify(signExtension(simm19), state);
    } else if (cond == 0xB && state->pstate.N != state->pstate.V) {
        //state->currAddress += offset;
        modify(signExtension(simm19), state);
    } else if (cond == 0xC && state->pstate.Z == 0 && state->pstate.N == state->pstate.V) {
        //state->currAddress += offset;
        modify(signExtension(simm19), state);
    } else if (cond == 0xD && (state->pstate.Z == 0 || state->pstate.N == state->pstate.V)) {
        //state->currAddress += offset;
        modify(signExtension(simm19), state);
    } else if (cond == 0xE) {
        //state->currAddress += offset;
        modify(signExtension(simm19), state);
    }
    //state->currAddress -= 4;
}


void B(uint32_t instruction, state *state) {


    //uint32_t reg1 = extractBits(instruction, 10, 31);
    //uint8_t reg2 = extractBits(instruction, 0, 4);
    //uint8_t condition1 = extractBits(instruction, 24, 31);
    //uint8_t condition2 = extractBits(instruction, 4, 4);
    uint32_t simm26 = extractBits(instruction, 0, 25);
    uint8_t xn = extractBits(instruction, 5, 9);
    uint32_t simm19 = extractBits(instruction, 5, 23);
    if (simm19 >> 18) {
      simm19 += 0xFFF80000;
    }
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
