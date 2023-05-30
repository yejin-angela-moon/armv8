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

static uint64_t readRegister (int registerIndex) {
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

void update_pstate(uint64_t result, uint64_t operand1, uint64_t operand2, bool is_subtraction) {
    // Update N and Z flags
    pstate.N = result & (1ULL << 63);  // check the most significant bit
    pstate.Z = (result == 0);

    if (is_subtraction) {
        // For subtraction, carry is set if operand1 >= operand2
        pstate.C = operand1 >= operand2;

        // Overflow for subtraction is set if operand1 and operand2 have different signs,
        // and operand1 and the result have different signs
        pstate.V = ((operand1 ^ operand2) & (operand1 ^ result)) >> 63;
    } else {
        // For addition, carry is set if result is less than either operand (meaning it wrapped around)
        pstate.C = result < operand1 || result < operand2;

        // Overflow for addition is set if operand1 and operand2 have the same sign,
        // and operand1 and the result have different signs
        pstate.V = (~(operand1 ^ operand2) & (operand1 ^ result)) >> 63;
    }
}

static void DPImm(uint32_t instruction) {
    
}
static void DPReg(uint32_t instruction) {

}
static void SDT(uint32_t instruction) {

}
static void LL(uint32_t instruction) {

}
static void B(uint32_t instruction) {
	#define signExtension(x) ((uint64_t)((uint32_t)x))
	uint8_t unconditional = extractBits(instruction, 26, 31);
	uint32_t reg1 = extractBits(instruction, 10, 31);
	uint8_t reg2 = extractBits(instruction, 0, 4);
	uint8_t condition1 = extractBits(instruction, 24, 31);
	uint8_t condition2 = extractBits(instruction, 4, 4);
	if (unconditional == 0x5) {
		//Unconditional
		uint32_t simm26 = extractBits(instruction, 0, 25);
		uint64_t offset = signExtension(simm26) * 4;
		PC = PC + offset;
	} else if (reg2 == 0x0 && reg1 == 0x3508160) {
		//Register
		uint8_t xn = extractBits(instruction, 5, 9);
		PC = readRegister(xn);
	} else if (condition2 == 0x0 && condition1 == 0x84) {
		//Conditional
		uint32_t simm19 = extractBits(instruction, 5, 23);
		uint64_t offset = signExtension(simm19) * 4;
		uint8_t cond = extractBits(instruction, 0, 3);
		if (cond == 0x0 && pstate.Z == 1) {
			PC = PC + offset;
		} else if (cond == 0x1 && pstate.Z == 0) {
			PC = PC + offset;
		} else if (cond == 0x6 && pstate.N == 1) {
			PC = PC + offset;
		} else if (cond == 0x7 && pstate.N != 1) {
			PC = PC + offset;
		} else if (cond == 0x12 && pstate.Z == 0 && pstate.N == pstate.V) {
			PC = PC + offset;
		} else if (cond == 0x13 && (pstate.Z == 0 || pstate.N == pstate.V)) {
			PC = PC + offset;
		} else if (cond == 0x14) {
			PC = PC + offset;
		} 
	}
}

/* Decode instruction */
static void readInstruction (uint32_t instruction) {
    if (extractBits(instruction, 26, 28) == 0b100){
        DPImm(instruction);
    }else if (extractBits(instruction, 25, 27) == 0b101)
    {
        DPReg(instruction);
    }else if(extractBits(instruction, 25, 28) == 0b1100 ){
        if (extractBits(instruction, 31,31) == 1){
            SDT(instruction);
        } else {
            LL(instruction);
        }
    } else if (instruction == 0x3573751839) {
	    //nop
	    PC = PC + 4;
    } else {
        B(instruction);
    }
}

int main(int argc, char **argv) {
  return EXIT_SUCCESS;
}
