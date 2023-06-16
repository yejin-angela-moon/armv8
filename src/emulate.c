#include "emulate.h"

/* Decode instruction, calling appropriate instruction type function */
static void readInstruction(uint32_t instruction, state *state) {
  if (instruction == NOP_INSTRUCTION) { // no operation
    return;
  }

  // IF statement to identify the type of instruction
  if (extractBits(instruction, 26, 28) == 4) {
    // Data Processing - Immediate
    DPImm(instruction, state);
  } else if (extractBits(instruction, 25, 27) == 5) {
    // Data Processing - Register
    DPReg(instruction, state);
  } else if (extractBits(instruction, 25, 28) == 12) {
    if (extractBits(instruction, 31, 31) == 1) {
      // Single Data Transfer
      SDT(instruction, state);
    } else {
      // Load Literal
      LL(instruction, state);
    }
  } else if (extractBits(instruction, 26, 28) == 5) {
    // Branch
    B(instruction, state);
  } else {
    fprintf(stderr, "invalid instruction\n");
    exit(1);
  }
}

static void execute(state *state) {
  uint32_t instruction = state->memory[0];
  int i = 0; // keeps track of normal execution PC value (without branching)
  while (1) {
    i++;

    if (instruction == HALT_INSTRUCTION) {
      break;
    }
    if (extractBits(instruction, 0, 4) == 0
        && extractBits(instruction, 10, 31) == BR_IDENTIFIER) {
      // BR is a special case as PC is not incremented (PC is set to address contained in register)
      B(instruction, state);
      instruction = state->memory[i];
    } else {
      readInstruction(instruction, state);
      inc_PC(state);
      instruction = state->memory[state->currAddress / 4];
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc < 2 || argc > 3) {
    printf("Usage: ./emulate <bin_file> [<out_file>]\n");
    return EXIT_FAILURE;
  }

  state *state = initialise();
  uint32_t *memory = state->memory;

  readFile(state, argv[1]);
  // reads and stores 32-bit chunks of binary input file into memory

  execute(state);
  // decodes (calls readInstruction()) and executes each 32-bit instruction in memory

  if (argc == 3) { // if output file present
    printStateToFile(state, argv[2]);
  } else {
    printToString(state);
  }

  free(state);
  free(memory);
  return EXIT_SUCCESS;
}


