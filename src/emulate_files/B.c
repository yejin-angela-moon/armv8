#include "B.h"

static void unconditional(uint32_t simm26, state *state) {
  int64_t offset = simm26 * 4;
  state->currAddress += offset;
}

static void reg(uint8_t xn, state *state) {
  state->currAddress = readRegister(xn, 1, state->generalRegisters);
}

static void conditional(uint32_t simm19, uint8_t cond, state *state) {
  //Pstate pstate = state->pstate;
  int64_t offset = simm19 * 4;
  if (cond == 0x0 && state->pstate.Z == 1) {
    state->currAddress += offset;
  } else if (cond == 0x1 && state->pstate.Z == 0) {
    state->currAddress += offset;
  } else if (cond == 0x6 && state->pstate.N == state->pstate.V) {
    state->currAddress += offset;
  } else if (cond == 0x7 && state->pstate.N != state->pstate.V) {
    state->currAddress += offset;
  } else if (cond == 0xC && state->pstate.Z == 0 && state->pstate.N == state->pstate.V) {
    state->currAddress += offset;
  } else if (cond == 0xD && (state->pstate.Z != 0 || state->pstate.N != state->pstate.V)) {
    state->currAddress += offset;
  } else if (cond == 0xE) {
    state->currAddress += offset;
  }
}


bool B(uint32_t instruction, state *state) {
  if (extractBits(instruction, 31, 31)) {
    reg(extractBits(instruction, 5, 9), state);
    return 0;
  } else if (extractBits(instruction, 30, 30)) {
    conditional(extractBits(instruction, 5, 23), extractBits(instruction, 0, 3), state);
    return 1;
  } else {
    unconditional(extractBits(instruction, 0, 25), state);
    return 1;
  }
}
