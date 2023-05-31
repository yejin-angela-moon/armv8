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

int64_t generalRegisters[NUM_REGISTERS];

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


static void writeRegister (int registerIndex, uint64_t newValue, bool sf) {
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

static uint64_t readRegister (int registerIndex, bool sf) {
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

static uint64_t extractBits(uint64_t n, uint8_t startIndex, uint8_t endIndex) {
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

void wide_move_immediate(uint8_t sf, uint8_t opc, uint32_t operand) {
    uint8_t hw = 0;
    uint16_t imm16 = operand & 0xFFFF;
}


static void DPImm(uint32_t instruction) {

}

static uint64_t unsignedOffset(bool sf, int offset, int baseRegister) {
    if (sf == 1){
        if (offset % 8 != 0 || offset > 32760){
            return 0;
        }
    } else{
        if (offset % 4 != 0 || offset > 16380){
            return 0;
        }
    }
    return readRegister(baseRegister, sf) + ((uint64_t) offset);
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
        default: ;
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
    uint8_t op2 = bitShift(shift, rm, operand);
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
        pstate.N = get_MSB(result); // but change size of vars?
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

// haven't implemented sf (64 vs 32-bit mode for registers)



static void SDT(uint32_t instruction, uint32_t *memory) {
    bool sf = extractBits(instruction, 30, 30);
    int offset = extractBits(instruction, 10, 21);
    int xn = extractBits(instruction, 5, 9);
    int rt = extractBits(instruction, 5, 9);
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
        val = readRegister(xn, sf) + simm;
        if (i == 1){
            addr = val;
        } else {
            addr = readRegister(xn, sf);
        }
        indexFlag = true;
    } else {
        //Register Offset
        //addr = xn + xm
        int xm = extractBits(instruction, 16, 20);
        addr = readRegister(xn, sf) + readRegister(xm, sf);
    }

    if (extractBits(instruction, 30, 30) == 1){
        if (extractBits(instruction, 22, 22) == 1){
            //load operation
            uint32_t wt;
            for (int i = 0; i < 3; i++){
                wt = wt | ((uint32_t) *(memory + (addr + i))) << 8*i;
            }

            writeRegister(rt, wt, sf);
        }else{
            //store operation
            uint32_t wt = (uint32_t) readRegister(rt, sf);
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

            writeRegister(rt, xt, sf);
        }else{
            //store operation
            uint64_t xt = (uint64_t) readRegister(rt, sf);
            for (int i = 0; i < 7; i++) {
                *(memory + (addr + i)) = extractBits(xt, 8*i + 7,8*i);
            }
        }
    }

    if (indexFlag){
        writeRegister(xn, val, sf);
    }
}

static void B(uint32_t instruction) {

}

static void LL(uint32_t instruction, uint32_t *memory) {
    int simm = extractBits(instruction, 5, 23);
    int rt = extractBits(instruction, 0, 4);
    bool sf = extractBits(instruction, 30, 30);
    int64_t offset = simm * 4;

    if (offset & (1<<18)){
        offset = offset | 0xFFFFFFFFFFFA0000;
    }

    writeRegister(rt, memory[currAddress + offset], sf);
}

/* Decode instruction */
static void readInstruction (uint32_t instruction, uint32_t *memory) {
    if (extractBits(instruction, 26, 28) == 0b100){
        DPImm(instruction);
    }else if (extractBits(instruction, 25, 27) == 0b101)
    {
        DPReg(instruction);
    }else if(extractBits(instruction, 25, 28) == 0b1100 ){
        if (extractBits(instruction, 31,31) == 1){
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
    pstate.Z = 1;
}

#define MEMORY_SIZE (2 * 1024 * 1024)  // 2 MiB

int main() {
    initialise();
    uint32_t* memory = (uint32_t*)malloc(MEMORY_SIZE * sizeof(uint32_t));
    // each element represents 1 byte of memory

    FILE* inputFile = fopen("input.bin", "rb");
    uint32_t instruction;
    do {
        fread(&instruction, sizeof(instruction), 1, inputFile);
        readInstruction(instruction, 1);
    } while (instruction != 0x8a000000);

    FILE *outputFile = fopen("emulateOutput.out", "w");
    for (int registerIndex = 0; registerIndex < NUM_REGISTERS; registerIndex++) {
        if (registerIndex < 10) {
            fprintf(outputFile, "X0%d = %016lx\n", registerIndex, readRegister(registerIndex, 1));
        } else {
            fprintf(outputFile, "X%d = %016lx\n", registerIndex, readRegister(registerIndex, 1));
        }
    }
    fprintf(outputFile, "PC = %016x\n", currAddress);
    fprintf(outputFile, "PSTATE : %s%s%s%s\n", pstate.N ? "N" : "-", pstate.Z ? "Z" : "-", pstate.C ? "C" : "-", pstate.V ? "V" : "-");
    fclose(outputFile);

    free(memory);
    return EXIT_SUCCESS;
}
