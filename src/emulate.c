#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#define NUMREGISTER 31
#define INITIALADDR 0x0

uint64_t generalRegisters[31]; // global var so initialised to 0s

typedef struct {
	bool N;
	bool Z;
	bool C;
	bool V;
} Pstate;

typedef struct {
	uint64_t generalRegisters[NUMREGISTER]; //General purpose registers R0-R30
	uint64_t ZR; //Zero register
	uint64_t PC; // Program Counter
	uint64_t SP; //Stack Pointer
	Pstate PSTATE; //Processor State Register
} Registers;

void initializeRegister(Registers *machine){
	for(int i = 0; i < NUMREGISTER; i++){
		machine->generalRegisters[i] = 0;
	}
	machine->ZR = 0;
	machine->PC = INITIALADDR;
	machine->SP = 0;
	machine->PSTATE.N = false;
	machine->PSTATE.Z = false;
	machine->PSTATE.C = false;
	machine->PSTATE.V = false;
}

typedef enum{
	DPImm,
	DPReg,
	SDT,
	LL,
	B
}instruction;

// global var so initialised to false's
struct pState PSTATE;

/* Private functions */

static int extractBits(uint32_t n, int startIndex, int endIndex) {
	// start/endIndex is inclusive, right-to-left starting from 0
	int mask = (1 << (endIndex - startIndex + 1)) - 1;
	return (n >> startIndex) & mask;
}

static void inc_PC (Registers *machine){
	machine->PC += 4;
}

static instruction readInstruction (uint32_t instruction) {
	if (extractBits(instruction, 26, 28) == 0b100){
		return DPImm;
	}else if (extractBits(instruction, 25, 27) == 0b101)
	{
		return DPReg;
	}else if(extractBits(instruction, 25, 28) == 0b1100 ){
		if (extractBits(instruction, 31,31) == 1){
			return SDT;
		} else {
			return LL;
		}
	} else {
		return B;
	}
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

int main(int argc, char **argv) {
  return EXIT_SUCCESS;
}
