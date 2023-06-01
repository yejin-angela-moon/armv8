#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include "ioutils.h"
#include "definition.h"

// ZR always returns 0. No fields needed.

uint32_t currAddress; // hexadecimal address to represents PC

// ZR always returns 0. No fields needed. 

// Initialise number of registers
uint64_t generalRegisters[NUM_REGISTERS];

//int generalRegisters[31]; // global var so initialised to 0s

Pstate pstate = {false, true, false, false}; // Z is initialised

/* Private functions */

static void inc_PC (){
    currAddress += 0x4;
}

static void writeRegister (int registerIndex, uint64_t newValue, uint8_t sf) {
    if (registerIndex == 31) {
        return;
    }
    if (sf == 0) {
        // Write to W-register: zero out the upper 32 bits
        generalRegisters[registerIndex] = newValue & 0xFFFFFFFF;
    } else {
        // Write to X-register: use the whole 64-bit value
        generalRegisters[registerIndex] = newValue;
    }
}

static uint64_t readRegister (int registerIndex, uint8_t sf) {
    if (registerIndex == 31) {
        return 0;
    }
    if (sf == 0) {
        // Read from W-register: return only the lower 32 bits
        return generalRegisters[registerIndex] & 0xFFFFFFFF;
    } else {
        // Read from X-register: return the whole 64-bit value
        return generalRegisters[registerIndex];
    }
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
	bool sh = (operand >> 17) & 0x01;     // extract bit 22
	uint16_t imm12 = (operand >> 5) & 0x0FFF; // extract bits 21-10
	uint8_t rn = operand & 0x1F;             // extract bits 9-5 
	uint64_t result = readRegister(rn, sf);

	if (sh) {
		imm12 <<= 12;
	} 
	
	// Operations performed depending on opc
	switch (opc) {
		case 0x0:
		//add
			result += imm12;
			break;
		case 0x1:
		//adds
			result += imm12;
			update_pstate(result, rn, imm12, 0);
			break;
		case 0x2:
		//sub
			result -= imm12;
			break;
		case 0x3:
		//subs
			result -= imm12;
			update_pstate(result, rn, imm12, 1);
		default:
			printf("Invalid opcode for arithmetic_immediate: %02X\n", opc);
			
	}
	// Store result in the destination register
	writeRegister(Rd, result, sf);
}
  
void wide_move_immediate(uint8_t sf, uint8_t opc, uint32_t operand, uint8_t Rd) {
	int hw = (operand >> 17) & 0x3;
    uint16_t imm16 = operand & 0xFFFF;

	uint64_t op = ((uint64_t)imm16) << (hw * 16); // calculate op

	switch (opc) {
		case 0x0: // movn
			writeRegister(Rd, ~op, sf);
			break;
		case 0x2: // movz
			writeRegister(Rd, op, sf);
			break;
		case 0x3: // movk
			{
				uint64_t mask = ~(0xFFFF << (hw * 16)) ;
				uint64_t value = readRegister(Rd, sf);
				value &= mask;
				if (sf == 1){
					uint64_t shiftedImm = imm16 << (hw * 16);
					value |= shiftedImm;
				}
				writeRegister(Rd, value, sf);
				break;
			}
		default:
			printf("Invalid opcode for wide_move_immediate: %02X\n", opc);
			exit(1);
	}
}
  
static void DPImm(uint32_t instruction) { // data processing instruction (immediate)
	uint8_t sf  = extractBits(instruction, 31, 31);        // extract bit 31
	uint8_t opc = extractBits(instruction, 29, 30);       // extract bits 30-29
	uint8_t opi = extractBits(instruction, 24, 25);       // extract bits 25-24
	uint32_t operand = extractBits(instruction, 5, 22); // extract bits 22-5
	uint8_t rd = extractBits(instruction, 0, 4);                // extract bits 4-0

	if (opi == 0x2) {
			arithmetic_immediate(sf, opc, operand, rd);
	} else if (opi == 0x5) {
			wide_move_immediate(sf, opc, operand, rd);
	}
}



static uint8_t bitShift(uint8_t shift, int64_t n, uint8_t operand) {
    switch (shift) {
        case 0:
            //lsl
            return n << operand;
        case 1:
            //lsr
            return n >> operand;
        case 2:
            //asr
            if (n < 0 && shift > 0) {
                return (n >> shift) | ~(~0U >> shift);
            } else {
                return n >> shift;
            }
        case 3: {
            //ror
            int bitCount = sizeof(n) * 8; // Calculates the total number of bits for the data type
            operand %= bitCount; // Just in case, reduce the number of rotations to a number less than bitCount
            return (n >> operand) | (n << (bitCount - operand));
        }
        default: return 0;
    }
}

static void arithmeticDPReg(uint8_t opc, uint8_t opr, uint8_t rd, uint8_t rn, uint8_t rm, uint8_t operand, bool sf) {
    uint8_t shift = extractBits(opr, 1, 2);
    uint8_t op2 = bitShift(shift, rm, operand);
    switch (opc) {
        case 0: {
            int result = readRegister(rn, sf) + op2;
            writeRegister(rd, result, sf);
            break;
        }
        case 1: {
            int result = readRegister(rn, sf) + op2;
            writeRegister(rd, result, sf);
            update_pstate(result,  rn, op2, 0);
            break;
        }
        case 2: {
            int result = readRegister(rn, sf) - op2;
            writeRegister(rd, result, sf);
            break;
        }
        case 3: {
            int result = readRegister(rn, sf) - op2;
            writeRegister(rd, result, sf);
            update_pstate(result, rn, op2, 1);
            break;
        }
        default: ;
    }

}

static unsigned int get_MSB(unsigned int num) {
    unsigned int MSB = 0;
    while (num != 0) {
        num = num / 2;
        MSB++;
    }
    return MSB;
}

static void logicalDPReg(uint8_t opc, uint8_t opr, uint8_t rd, uint8_t rn, uint8_t rm, uint8_t operand, bool sf) {
    uint8_t shift = extractBits(opr, 1, 2);
    uint64_t op2 = bitShift(shift, rm, operand);
    bool n = opr % 2;

    if (opc == 0) {
        if (n) {
            writeRegister(rd, readRegister(rn, sf) & ~op2, sf);
        } else {
            writeRegister(rd, readRegister(rn, sf) & op2, sf);
        }
    } else if (opc == 1) {
        if (n) {
            writeRegister(rd, readRegister(rn, sf) | ~op2, sf);
        } else {
            writeRegister(rd, readRegister(rn, sf) | op2, sf);
        }
    } else if (opc == 2) {
        if (n) {
            writeRegister(rd, readRegister(rn, sf) ^ ~op2, sf);
        } else {
            writeRegister(rd, readRegister(rn, sf) ^ op2, sf);
        }
    } else if (opc == 3) {
        int result;
        if (n) {
            result = readRegister(rn, sf) & ~op2;
            writeRegister(rd, result, sf);
        } else {
            result = readRegister(rn, sf) & op2;
            writeRegister(rd, result, sf);
        }
        pstate.N = get_MSB(result);
        pstate.Z = result == 0;
        pstate.C = 0;
        pstate.V = 0;
    }
}

static void multiplyDPReg(uint32_t instruction, uint8_t rd, uint8_t rn, uint8_t rm, uint8_t operand, bool sf) {
    bool x = extractBits(operand, 5, 5);
    uint8_t ra = extractBits(operand, 0, 4);

    if (x) {
        writeRegister(rd, readRegister(ra, sf) + readRegister(rn, sf) * readRegister(rm, sf), sf);
    } else {
        writeRegister(rd, readRegister(ra, sf) - readRegister(rn, sf) * readRegister(rm, sf), sf);
    }
}

static void DPReg(uint32_t instruction) {
    uint8_t rd = extractBits(instruction, 0, 4);
    uint8_t rn = extractBits(instruction, 5, 9);
    uint8_t rm = extractBits(instruction, 16, 20);
    uint8_t operand = extractBits(instruction, 10, 15);
    bool sf = extractBits(instruction, 31, 31);
    uint8_t opc = extractBits(instruction, 29, 30);
    bool m = extractBits(instruction, 28, 28);
    uint8_t opr = extractBits(instruction, 21, 24);

    if (m == 1) {
        multiplyDPReg(instruction, rd, rn, rm, operand, sf);
    } else {
        if (extractBits(opr, 4, 4)) {
            arithmeticDPReg(opc, opr, rd, rn, rm, operand, sf);
        } else {
            logicalDPReg(opc, opr, rd, rn, rm, operand, sf);
        }
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
		currAddress = readRegister(xn, 0);
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

/* Decode instruction */
static void readInstruction (uint32_t instruction, uint32_t *memory) {
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
            //SDT(instruction, memory);
        } else {
            //LL(instruction, memory, currAddress);
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
    pstate.Z = 1; 
}

void readFile(uint32_t* memory, char* filename){
    FILE *fp = fopen(filename, "rb");
    int fileSize;

    if (fp == NULL){
        fprintf(stderr, "can't opern %s/n", filename);
        exit(1);
    }

    fseek(fp, 0, SEEK_END);
    fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    fread(memory, sizeof (int32_t), fileSize/sizeof (int32_t), fp);

/*
    uint32_t instruction;
    do {
        fread(memory, fileSize, 1, fp);
        fread(&instruction, fileSize, 1, fp);
        readInstruction(instruction, memory);
        inc_PC();
    } while (instruction != HALT_INSTRUCTION);

    */

    fclose(fp);
}

void printStateToFile( uint32_t* memory, char* filename, Pstate pstate, uint32_t currAddress){
    FILE *outputFile = fopen(filename, "w");

    if (outputFile == NULL){
        printf("Error opeing file\n");
        exit(1);
    }

    //print registers
    fprintf(outputFile, "Register:\n");
    for(int i = 0; i < NUM_REGISTERS; i++ ){
        if (i < 10) {
            fprintf(outputFile, "X0%d = %016lx\n", i, readRegister(i, 0));
        } else {
            fprintf(outputFile, "X%d = %016lx\n", i, readRegister(i, 0));
        }
    }

    //Print PC
    fprintf(outputFile, "PC = %08x\n", currAddress);
    fprintf(outputFile, "PSTATE : %s%s%s%s\n",
            pstate.N ? "N" : "-",
            pstate.Z ? "Z" : "-",
            pstate.C ? "C" : "-",
            pstate.V ? "V" : "-");

    //print non-zero memory
    fprintf(outputFile, "Non-zero memory:\n");
    for (int i = 0; i < MEMORY_SIZE; i += 4) {
        if (memory[i] != 0) {
            fprintf(outputFile, "0x%08x: %08x\n", i, memory[i]);
        }
    }

    fclose(outputFile);
}

void printToString( uint32_t* memory, Pstate pstate, uint32_t currAddress){

    //print registers
    printf("Register:\n");
    for(int i = 0; i < NUM_REGISTERS; i++ ){
        if (i < 10) {
            printf("X0%d = %016lx\n", i, readRegister(i, 0));
        } else {
            printf("X%d = %016lx\n", i, readRegister(i, 0));
        }
    }

    //Print PC
    printf("PC = %08x\n", currAddress);
    printf("PSTATE : %s%s%s%s\n",
           pstate.N ? "N" : "-",
           pstate.Z ? "Z" : "-",
           pstate.C ? "C" : "-",
           pstate.V ? "V" : "-");

    //print non-zero memory
    printf("Non-zero memory:\n");
    for (int i = 0; i < MEMORY_SIZE; i += 4) {
        if (memory[i] != 0) {
            printf("0x%08x: %08x\n", i, memory[i]);
        }
    }
}


static void execute(uint32_t* memory){
	uint32_t instruction;
    while (1){
        if (instruction == HALT_INSTRUCTION){
            break;
        }
        instruction = memory[currAddress / 4];
        readInstruction(instruction, memory);
        inc_PC();
    }
}

int main(int argc, char* argv[]) {

	if (argc < 2 || argc > 3){
		printf("Usage: ./emulate <bin_file> [<out_file>]\n");
        return 1;
	}

    initialise();

    uint32_t *memory = (uint32_t*)calloc(MEMORY_SIZE, sizeof(uint32_t));
	
	readFile(memory, argv[1]);
	
	execute(memory);

	if (argc == 3){
		printStateToFile(memory, argv[2], pstate, currAddress);
	} else{
		printToString(memory, pstate, currAddress);
	}
    
    free(memory);
    return EXIT_SUCCESS;
}
