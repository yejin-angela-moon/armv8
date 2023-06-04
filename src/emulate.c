#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#include "emulate_files/utilities.h"
#include "emulate_files/DPImm.h"
#include "emulate_files/DPReg.h"
#include "emulate_files/SDT.h"
#include "emulate_files/B.h"
#include "emulate_files/ioutils.h"
#include "emulate_files/definitions.h"

/* Decode instruction */
void readInstruction (uint32_t instruction, state *state) {
    if (instruction == NOP_INSTRUCTION) { //nop
        inc_PC(state);
        return;
    }
    if (extractBits(instruction, 26, 28) == 0x4){
        printf("DPI\n");
        DPImm(instruction, state);
    } else if (extractBits(instruction, 25, 27) == 0x5) {
        printf("DPR\n");
        DPReg(instruction, state);
    } else if(extractBits(instruction, 25, 28) == 0xC ){
        if (extractBits(instruction, 31,31) == 0x1){
            printf("SDT\n");
            SDT(instruction, state);
        } else {
            printf("LL\n");
            LL(instruction, state);
        }
    } else {
        printf("B\n");
        B(instruction, state);
    }
}

static void execute(state* state){
    uint32_t instruction;
    while (1){
        instruction = state->memory[state->currAddress / 4];
        if (instruction == HALT_INSTRUCTION){
            break;
        }
        readInstruction(instruction, state);
        inc_PC(state);
    }
}


int main(int argc, char* argv[]) {

    if (argc < 2 || argc > 3){
        printf("Usage: ./emulate <bin_file> [<out_file>]\n");
        return EXIT_FAILURE;
    }

    state *state = initialise();
    uint32_t *memory = state->memory;

    readFile(state, argv[1]);

    execute(state);

    if (argc == 3){
        printStateToFile(state, argv[2]);
    } else{
        printToString(state);
    }

    free(state);
    free(memory);
    return EXIT_SUCCESS;
}
