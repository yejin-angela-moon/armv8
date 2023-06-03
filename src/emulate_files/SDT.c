#include "SDT.h"

void SDT(uint32_t instruction, state *state) {
    bool sf = extractBits(instruction, 30, 30);
    uint32_t offset = extractBits(instruction, 10, 21);
    uint32_t xn = extractBits(instruction, 5, 9);
    uint32_t rt = extractBits(instruction, 0, 4);
    uint32_t addr;
    bool indexFlag = false;
    uint64_t val;
    uint64_t *generalRegisters = state->generalRegisters;
    uint32_t *memory = state->memory;
 
    if (extractBits(instruction, 24, 24) == 1){
        //Unsigned Immediate Offset when U = 1, addr = xn + imm12
        addr = readRegister(xn, 0, generalRegisters) + (sf? offset << 3: offset << 2);
    } else if (extractBits(instruction, 21, 21) == 0){
        //Pre/Post-Index
        //when i = 1 (pre indexed), addr = xn + simm9 and xn = xn + simm9
        //when i = 0 (post indexed), addr = xn and xn = xn + simm9
        uint32_t simm = extractBits(instruction, 12, 20);
        uint32_t i = extractBits(instruction, 11, 11);
        val = readRegister(xn, 0, generalRegisters) + simm;
        if (i == 1){
            addr = val;
        } else {
            addr = readRegister(xn, 0, generalRegisters);
        }
        indexFlag = true;
    } else {
        //Register Offset
        //addr = xn + xm
        uint32_t xm = extractBits(instruction, 16, 20);
        addr = readRegister(xn, 0, generalRegisters) + readRegister(xm, 0, generalRegisters);
    }

    uint16_t *twoByteMem = (uint16_t*)(&memory[addr/4]);
    if (addr%4 != 0){
        twoByteMem = &twoByteMem[1];
    }

    if (sf == 0){
        if (extractBits(instruction, 22, 22) == 1){
            //load operation

            uint32_t wt = 0;
            for (int i = 0; i < 3; i++){
                wt |= ((uint32_t) twoByteMem[i] << (16*i));
            }
            writeRegister(rt, wt, sf, generalRegisters);

        }else{
            //store operation
            uint32_t wt = (uint32_t) readRegister(rt, sf, generalRegisters);
            for (int i = 0; i < 3; i++){
                twoByteMem[i] = (wt>>(i*16)) & 0xFFFF;
            }
        }
    } else {
        if (extractBits(instruction, 22, 22) == 1){
            //load operation
            uint64_t xt = 0;
            for (int i = 0; i < 4; i++){
                xt |= (((uint64_t) twoByteMem[i] )  << (16*i));
            }
            writeRegister(rt, xt, sf, generalRegisters);
        }else{
            //store operation
            uint64_t xt = (uint64_t) readRegister(rt, sf, generalRegisters);
            memory[addr/4] = xt;
        }
    }

    if (indexFlag){
        writeRegister(xn, val, sf, generalRegisters);
    }
}

void LL(uint32_t instruction, state *state) {
    bool sf = extractBits(instruction, 30,30);
    uint32_t simm = extractBits(instruction, 5, 23);
    uint32_t rt = extractBits(instruction, 0, 4);
    int64_t offset = simm * 4;
    uint32_t *memory = state->memory;
    uint32_t currAddress = state->currAddress;
    if (sf) {
        int64_t xt = 0;
        for (int i =0; i < 2; i++){
            xt |= (((uint64_t) memory[(currAddress + offset)/4 + i]) << (32 * i));
        }
        writeRegister(rt, xt, sf, state->generalRegisters);
    } else {
        writeRegister(rt, memory[state->currAddress + offset], sf, state->generalRegisters);
    }

}
