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

//int generalRegisters[31]; // global var so initialised to 0s

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

void update_pstate(Pstate* pstate, uint64_t result, uint64_t rd, uint8_t rn, uint32_t op2, bool is_sub) {
	// Update N (Negative Flag): Set to the sign bit of the result
    	pstate->N = result & (1ULL << 31);

	// Update Z (Zero Flag): Set only when the result is all zero
    	pstate->Z = (result == 0);

	if (is_sub) {
        	// In a subtraction, C is set to zero if the subtraction produces a borrow, otherwise it is set to one
        	pstate->C = (rn >= op2);
		
    	} else {
        	// In an addition, C is set when the addition produces a carry (unsigned overflow), or zero otherwise
        	pstate->C = (result < rn);
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
    } else {
        B(instruction);
    }
}


/* Data Processing Instructions */

void arithmetic_immediate(uint8_t sf, uint8_t opc, uint32_t operand, uint8_t Rd)  {
	uint8_t sh = (operand >> 17) & 0x01;     // extract bit 22
        uint16_t imm12 = (operand >> 5) & 0x0FFF; // extract bits 21-10
        uint8_t rn = operand & 0x1F;             // extract bits 9-5 
	uint64_t result;
	
	// Store result in the destination register
      	if (sf) {
		imm12 <<= 12;
        	generalRegisters[Rd] = result;  // 64-bit
	
      	} else {
        	generalRegisters[Rd] = (uint32_t)result;  // 32-bit
      	}
  
      	// Operations performed depending on opc
      	switch (opc) {
		case 0x0:
	  		result += rn;
		case 0x1:
			result += rn;
			update_pstate(&pstate, result,  Rd, rn, imm12, 0);
		case 0x2:
			result -= rn;
		case 0x3:
			result -= rn;
			update_pstate(&pstate, result, Rd, rn, imm12, 1);
	}
  
}
  
void wide_move_immediate(uint8_t sf, uint8_t opc, uint32_t operand) {
          uint8_t hw = (operand >> 17) & 0x03;
          uint16_t imm16 = operand & 0xFFFF;
  }

void data_process_immediate(uint32_t instruction) {
	uint8_t sf  = extractBits(instruction, 31, 31);        // extract bit 31
    	uint8_t opc = extractBits(instruction, 29, 30);       // extract bits 30-29
    	uint8_t opi = extractBits(instruction, 24, 25);       // extract bits 25-24
    	uint32_t operand = extractBits(instruction, 5, 22); // extract bits 22-5
    	uint8_t rd = extractBits(instruction, 0, 4);                // extract bits 4-0

	if (opi == 0x2) {
		arithmetic_immediate(sf, opc, operand, rd);
	} else if (opi == 0x5) {
		wide_move_immediate(sf, opc, operand);
	}
}

int main(int argc, char **argv) {
  return EXIT_SUCCESS;
}
