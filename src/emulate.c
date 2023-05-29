#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

/*registers*/

uint32_t currAddress; // hexadecimal address to represents PC

// ZR always returns 0. No fields needed.

// Initialise number of registers
#define NUM_REGISTERS 31

uint64_t generalRegisters[NUM_REGISTERS];

// Initialise memory size
#define MEMORY_SIZE (1024 * 1024)  // 1 MiB, for example

uint8_t memory[MEMORY_SIZE];  // each element represents 1 byte of memory
// but could also be 2D array?

void initialise_memory() {
    memset(memory, 0, sizeof(memory));
} // later

typedef struct {
    bool N;
    bool Z;
    bool C;
    bool V;
} Pstate;

Pstate pstate;

/* Private functions */

static void inc_PC (){
    currAddress += 4;
}

static int readRegister (int registerIndex) {
    // registerIndex = 11111 (bin) -> reading from ZR
    if (registerIndex == 31) {
        return 0;
    }
    return generalRegisters[registerIndex];
}

static void writeRegister (int registerIndex, int newValue) {
    // registerIndex = 11111 (bin) -> writing to ZR
    if (registerIndex == 31) {
        return;
    }
    generalRegisters[registerIndex] = newValue;
}

static int extractBits(uint32_t n, int startIndex, int endIndex) {
    // start/endIndex is inclusive, right-to-left starting from 0
    int mask = (1 << (endIndex - startIndex + 1)) - 1;
    return (n >> startIndex) & mask;
}

int main(int argc, char **argv) {
  return EXIT_SUCCESS;
}
