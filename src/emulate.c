#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

/*registers*/

int currAddress = 0x0; // this represents PC

// ZR not needed as no fields

// initialise registers
#define NUM_REGISTERS 31

uint64_t registers[NUM_REGISTERS];

void initialize_registers() {
    for (int i = 0; i < NUM_REGISTERS; ++i) {
        registers[i] = 0;
    }
}

// initialise memory
#define MEMORY_SIZE 1024 * 1024  // 1 MiB, for example

uint8_t memory[MEMORY_SIZE];  // each element represents 1 byte of memory

void initialize_memory() {
    memset(memory, 0, sizeof(memory));
}

//int generalRegisters[31]; // global var so initialised to 0s

typedef struct {
	bool N;
	bool Z;
	bool C;
	bool V;
} Pstate;

Pstate pstate = {false, false, false, false}; // initialisation of pState

/* Private functions */

static void inc_PC (){
	currAddress += 4;
}

static void readInstruction ( int objectCode ) {
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


/* Data Processing Instructions */

// ADD instruction with register
void add_reg(int Rd, int Rn, int Rm) {
    registers[Rd] = registers[Rn] + registers[Rm];
    update_pstate(registers[Rd], Rd, Rn, Rm);
}

// ADD instruction with immediate value
void arithmetic_imm(int opc, int sf, int Rd, int Rn, int sh, uint32_t imm12) {

    // Initialize operand based on imm12 and sh
    uint64_t operand = (sh == 1) ? imm12 << 12 : imm12;

    // Initialize result based on the opcode and the values of registers[Rn] and operand
    uint64_t result;      

    switch (opc) {
        case 0: // add
        case 1: // adds
            result = registers[Rn] + operand;
            break;
        case 2: // sub
        case 3: // subs
            result = registers[Rn] - operand;
            break;
    }

    // Store result in the destination register
    if (sf) {
        registers[Rd] = result;  // 64-bit
    } else {
        registers[Rd] = (uint32_t)result;  // 32-bit
    }

    // Update condition flags for 'adds' and 'subs' instructions
    if (opc == 1 || opc == 3) {
        bool is_subtraction = (opc == 2 || opc == 3);
        update_pstate(result, registers[Rn], operand, is_subtraction);
    }	
}


/* Single Data Transfer Instructions */

// Load instruction (LDR)
void ldr(int Rt, uint64_t address) {
    registers[Rt] = *((uint64_t*)(memory + address));
}

// Store instruction (STR)
void str(int Rt, uint64_t address) {
    *((uint64_t*)(memory + address)) = registers[Rt];
}

/* Branch Instructions */

// Branch instruction
void b(int64_t offset) {
    currAddress += offset;
}





int main(int argc, char **argv) {
  return EXIT_SUCCESS;
}
