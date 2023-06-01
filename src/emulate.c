#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#include "utilities.h"
#include "definitions.h"
#include "DPReg.h"

static void DPImm(uint32_t instruction) {

}

static void SDT(uint32_t instruction, uint32_t *memory) {
}

static void B(uint32_t instruction) {
}

static void LL(uint32_t instruction, uint32_t *memory) {
}

/* Decode instruction */
void readInstruction (uint32_t instruction, state *state) {
    if (instruction == 0xD503201F) {
        //nop
        inc_PC(state);
        return;
    }
    if (extractBits(instruction, 26, 28) == 0x4){
        DPImm(instruction, state);
    } else if (extractBits(instruction, 25, 27) == 0x5) {

        DPReg(instruction, state);
    } else if(extractBits(instruction, 25, 28) == 0xC ){
        if (extractBits(instruction, 31,31) == 0x1){
            SDT(instruction, state);
        } else {
            LL(instruction, state);
        }
    } else {
        B(instruction);
    }
}

int main(int argc, char* argv[]) {

    if (argc < 2 || argc > 3){
        printf("Usage: ./emulate <bin_file> [<out_file>]\n");
        return EXIT_FAILURE;
    }

    state *state;
    initialise(state);
    uint32_t *memory = state->memory;

    readFile(memory, argv[1]);

    execute(memory, state);

    if (argc == 3){
        printStateToFile(memory, argv[2]);
    } else{
        printToString(memory);
    }

    free(memory);
    return EXIT_SUCCESS;
}
