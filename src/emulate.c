#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

/*registers*/

int currAddress = 0x0; // this represents PC

// ZR not needed as no fields

int generalRegisters[31]; // global var so initialised to 0s

struct pState {
	bool N;
	bool Z;
	bool C;
	bool V;
}

/* Private functions */

static int extractBits(int n, int startIndex, int endIndex) {
	// start/endIndex is inclusive, right-to-left starting from 0
	int mask = (1 << (endIndex - startIndex + 1)) - 1;
	return (n >> startIndex) & mask;
}

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

int main(int argc, char **argv) {
  return EXIT_SUCCESS;
}
