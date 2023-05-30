#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

// ZR always returns 0. No fields needed.

uint32_t currAddress; // hexadecimal address to represents PC

// ZR always returns 0. No fields needed. 

// Initialise number of registers
#define NUM_REGISTERS 31

uint64_t generalRegisters[NUM_REGISTERS];

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

static int extractBits(uint64_t n, int startIndex, int endIndex) {
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


void arithmetic_immediate(uint8_t sf, uint8_t opc, uint32_t operand, uint8_t Rd)  {
	uint8_t sh = (operand >> 17) & 0x01;     // extract bit 22
        uint16_t imm12 = (operand >> 5) & 0x0FFF; // extract bits 21-10
        uint8_t rn = operand & 0x1F;             // extract bits 9-5 
        uint64_t result = 0;
  
        // Store result in the destination register
        if (sh) {
                imm12 <<= 12;
                writeRegister(Rd, result);  // 64-bit
        } else {
                generalRegisters[Rd] = (uint32_t)result;  // 32-bit
        }
  
        // Operations performed depending on opc
        switch (opc) {
                case 0x0:
                        result += rn;
			break;
                case 0x1:
                        result += rn;
                        update_pstate(result, rn, imm12, 0);
			break;
                case 0x2:
                        result -= rn;
			break;
                case 0x3:
                        result -= rn;
                        update_pstate(result, rn, imm12, 1);
			break;
        }
}
  
void wide_move_immediate(uint8_t sf, uint8_t opc, uint32_t operand) {
        //    uint8_t hw = (operand >> 17) & 0x03;
          //  uint16_t imm16 = operand & 0xFFFF;
	
    }
  
static void DPImm(uint32_t instruction) {
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



static void DPReg(uint32_t instruction) {

}

static uint64_t unsignedOffset(int sf, int offset, int baseRegister){
	if (sf == 1){
		if (offset % 8 != 0 || offset > 32760){
			return 0;
		}
	} else{
		if (offset % 4 != 0 || offset > 16380){
			return 0;
		}
	}
	return readRegister(baseRegister) + ((uint64_t) offset);
}

static void SDT(uint32_t instruction, uint64_t *memory) {
	int sf = extractBits(instruction, 30, 30);
	int offset = extractBits(instruction, 10, 21);
	int xn = extractBits(instruction, 5, 9);
	int rt = extractBits(instruction, 0, 4);
	uint64_t addr;
	bool indexFlag = false;
	int val;
	
	if (extractBits(instruction, 24, 24) == 1){
		//Unsigned Immediate Offset when U = 1, addr = xn + imm12
		addr = unsignedOffset(sf, offset, xn);
	} else if (extractBits(instruction, 21, 21) == 0){
		//Pre/Post-Index
		//when i = 1 (pre indexed), addr = xn + simm9 and xn = xn + simm9
		//when i = 0 (post indexed), addr = xn and xn = xn + simm9
		int simm = extractBits(instruction, 12, 20);
		int i = extractBits(instruction, 11, 11);
		val = readRegister(xn) + simm;
		if (i == 1){
			addr = val;
		} else {
			addr = readRegister(xn);
		}
		indexFlag = true;
	} else {
		//Register Offset
		//addr = xn + xm
		int xm = extractBits(instruction, 16, 20);
		addr = readRegister(xn) + readRegister(xm);
	}
	
	if (extractBits(instruction, 30, 30) == 1){
		if (extractBits(instruction, 22, 22) == 1){
			//load operation
			uint32_t wt;
			for (int i = 0; i < 3; i++){
				wt = wt | ((uint32_t) *(memory + (addr + i))) << 8*i;
			}

			writeRegister(rt, wt);
		}else{
			//store operation
			uint32_t wt = (uint32_t) readRegister(rt);
			for (int i = 0; i < 3; i++){
				*(memory + (addr + i)) = extractBits(wt, 8*i + 7,8*i);
			} 
		}
	} else {
		if (extractBits(instruction, 22, 22) == 1){
			//load operation
			uint64_t xt;
			for (int i = 0; i < 7; i++){
				xt = xt | ((uint64_t) *(memory + (addr + i))) << 8*i;
			}

			writeRegister(rt, xt);
		}else{
			//store operation
			uint64_t xt = (uint64_t) readRegister(rt);
			for (int i = 0; i < 7; i++) {
				*(memory + (addr + i)) = extractBits(xt, 8*i + 7,8*i);
			}
		}
	}

	if (indexFlag){
		writeRegister(xn, val);
	}
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
		currAddress += offset;
	} else if (reg2 == 0x0 && reg1 == 0x3587c0) {
		//Register
		uint8_t xn = extractBits(instruction, 5, 9);
		currAddress = readRegister(xn);
	} else if (condition2 == 0x0 && condition1 == 0x54) {
		//Conditional
		uint32_t simm19 = extractBits(instruction, 5, 23);
		uint64_t offset = signExtension(simm19) * 4;
		uint8_t cond = extractBits(instruction, 0, 3);
		if (cond == 0x0 && pstate.Z == 1) {
			currAddress += offset;
		} else if (cond == 0x1 && pstate.Z == 0) {
			currAddress += offset;
		} else if (cond == 0x6 && pstate.N == 1) {
			currAddress += offset;
		} else if (cond == 0x7 && pstate.N != 1) {
			currAddress += offset;
		} else if (cond == 0xC && pstate.Z == 0 && pstate.N == pstate.V) {
			currAddress += offset;
		} else if (cond == 0xD && (pstate.Z == 0 || pstate.N == pstate.V)) {
			currAddress += offset;
		} else if (cond == 0xE) {
			currAddress += offset;
		} 
	}
}

static void LL(uint32_t instruction, uint64_t *memory) {
	int simm = extractBits(instruction, 5, 23);
	int rt = extractBits(instruction, 0, 4);
	int64_t offset = simm * 4;

	if (offset & (1<<18)){
		offset = offset | 0xFFFFFFFFFFFA0000;
	}

	writeRegister(rt, *(memory + currAddress + offset));
}


/* Decode instruction */
static void readInstruction (uint32_t instruction, uint64_t *memory) {
    if (instruction == 0xD503201F) {
	//nop
	inc_PC();
	return;
    }
    if (extractBits(instruction, 26, 28) == 0x4){
        DPImm(instruction);
    } else if (extractBits(instruction, 25, 27) == 0x5) {
    
        DPReg(instruction);
    } else if(extractBits(instruction, 25, 28) == 0xC ){
        if (extractBits(instruction, 31,31) == 0x1){
            SDT(instruction, memory);
        } else {
            LL(instruction, memory);
        }
    } else {
        B(instruction);
    }
}


static void initialise() {
    currAddress = 0;
    memset(generalRegisters, 0, sizeof(generalRegisters));
    pstate.C = 0;
    pstate.N = 0;
    pstate.V = 0;
    pstate.Z = 0;
}

#define MEMORY_SIZE (2 * 1024 * 1024)  // 2 MiB

int main() {
    initialise();
    uint64_t* memory = (uint64_t*)malloc(MEMORY_SIZE * sizeof(uint64_t));
    // each element represents 1 byte of memory

    FILE* inputFile = fopen("input.bin", "rb");
    uint32_t instruction;
    do {
        fread(&instruction, sizeof(instruction), 1, inputFile);
        readInstruction(instruction, memory);
    } while (instruction != 0x8a000000);

    FILE *outputFile = fopen("emulateOutput.out", "w");
    for (int registerIndex = 0; registerIndex < NUM_REGISTERS; registerIndex++) {
        if (registerIndex < 10) {
            fprintf(outputFile, "X0%d = %016lx\n", registerIndex, readRegister(registerIndex));
        } else {
            fprintf(outputFile, "X%d = %016lx\n", registerIndex, readRegister(registerIndex));
        }
    }
    fprintf(outputFile, "PC = %016x\n", currAddress);
    fprintf(outputFile, "PSTATE : %s%s%s%s\n", pstate.N ? "N" : "-", pstate.Z ? "Z" : "-", pstate.C ? "C" : "-", pstate.V ? "V" : "-");
    fclose(outputFile);

    free(memory);
    return EXIT_SUCCESS;
}
