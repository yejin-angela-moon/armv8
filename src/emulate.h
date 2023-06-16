#ifndef ARMV8_48_EMULATE_H
#define ARMV8_48_EMULATE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "emulate_files/utilities.h"
#include "emulate_files/DPImm.h"
#include "emulate_files/DPReg.h"
#include "emulate_files/SDT.h"
#include "emulate_files/B.h"
#include "emulate_files/ioutils.h"
#include "emulate_files/definitions.h"

static void readInstruction(uint32_t instruction, state *state);

static void execute(state *state);

int main(int argc, char *argv[]);

#endif //ARMV8_48_EMULATE_H
