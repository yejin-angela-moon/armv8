#include "B.h"

static void modify(int64_t offset, state *state) {
  if (offset < 0) {
    state->currAddress -= labs(offset) * 4;
  } else {
    state->currAddress += offset * 4;
  }
  state->currAddress -= 4;
}

static void unconditional(int64_t simm26, state *state) {
  // b
  modify(simm26, state);
}

static void reg(uint8_t xn, state *state) {
  // br
  state->currAddress = readRegister(xn, 1, state->generalRegisters);
}

static void conditional(int32_t simm19, uint8_t cond, state *state) {
  // b.cond
  Pstate pstate = state->pstate;

  bool shouldModify =
          (cond == 0 && pstate.Z) ||
          (cond == 1 && pstate.Z == 0) ||
          (cond == 0xA && pstate.N == pstate.V) ||
          (cond == 0xB && pstate.N != pstate.V) ||
          (cond == 0xC && pstate.Z == 0 && pstate.N == pstate.V) ||
          (cond == 0xD && (pstate.Z == 0 || pstate.N == pstate.V)) ||
          cond == 0xE;

  if (shouldModify) {
    modify(simm19, state);
  }
}

void B(uint32_t instruction, state *state) {
  int64_t simm26 = extractBits(instruction, 0, 25);
  uint8_t xn = extractBits(instruction, 5, 9);
  int32_t simm19 = extractBits(instruction, 5, 23);
  uint8_t cond = extractBits(instruction, 0, 3);

  if (simm26 >> 25) {
    simm26 += SIGN_EXTEND_26BITS;
  }

  if (simm19 >> 18) {
    simm19 += SIGN_EXTEND_19BITS;
  }

  if (extractBits(instruction, 0, 4) == 0 && extractBits(instruction, 10, 31) == BR_IDENTIFIER) {
    reg(xn, state);
  } else if (extractBits(instruction, 4, 4) == 0 && extractBits(instruction, 24, 31) == 0x54) {
    conditional(simm19, cond, state);
  } else {
    unconditional(simm26, state);
  }
}
