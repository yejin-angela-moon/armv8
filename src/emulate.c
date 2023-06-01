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
static void readInstruction (uint32_t instruction) {
    if (instruction == 0xD503201F) {
	//nop
	inc_PC();
	return;
    }
    if (extractBits(instruction, 26, 28) == 0b100){
        DPImm(instruction);
        inc_PC();
    }else if (extractBits(instruction, 25, 27) == 0b101)
    {
        DPReg(instruction);
        inc_PC();
    }else if(extractBits(instruction, 25, 28) == 0b1100){
        if (extractBits(instruction, 31,31) == 1){
            SDT(instruction);
            inc_PC();
        } else {
            LL(instruction);
            inc_PC();
        }
    } else {
        B(instruction);
    }
}

int main(int argc, char* argv[]) {

    if (argc < 2 || argc > 3){
        printf("Usage: ./emulate <bin_file> [<out_file>]\n");
        return 1;
    }

    state *state;
    initialise(state);

    uint32_t *memory = (uint32_t*)calloc(MEMORY_SIZE, sizeof(uint32_t));

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
