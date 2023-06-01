#include <stdint.h>
#include <stdbool.h>

void SDT(uint32_t instruction, uint32_t* memory) {
	int sf = extractBits(instruction, 30, 30);
	int offset = extractBits(instruction, 10, 21);
	int xn = extractBits(instruction, 5, 9);
	int rt = extractBits(instruction, 0, 4);
	uint32_t addr;
	bool indexFlag = false;
	int val;
	
	if (extractBits(instruction, 24, 24) == 1){
		//Unsigned Immediate Offset when U = 1, addr = xn + imm12
		addr = readRegister(xn, 0) + offset;
	} else if (extractBits(instruction, 21, 21) == 0){
		//Pre/Post-Index
		//when i = 1 (pre indexed), addr = xn + simm9 and xn = xn + simm9
		//when i = 0 (post indexed), addr = xn and xn = xn + simm9
		int simm = extractBits(instruction, 12, 20);
		int i = extractBits(instruction, 11, 11);
		val = readRegister(xn, 0) + simm;
		if (i == 1){
			addr = val;
		} else {
			addr = readRegister(xn, 0);
		}
		indexFlag = true;
	} else {
		//Register Offset
		//addr = xn + xm
		int xm = extractBits(instruction, 16, 20);
		addr = readRegister(xn, 0) + readRegister(xm, 0);
	}
	if (sf == 0){
		if (extractBits(instruction, 22, 22) == 1){
			//load operation
			uint32_t wt = 0;
			for (int i = 0; i < 3; i++){
				wt |= ((uint32_t) memory[addr + i]) << (8*i);
			}
			writeRegister(rt, wt, sf);
		}else{
			//store operation
			uint32_t wt = (uint32_t) readRegister(rt, sf);
			for (int i = 0; i < 3; i++){
				memory[addr + i] = extractBits(wt, 8*i + 7,8*i);
			} 
		}
	} else {
		if (extractBits(instruction, 22, 22) == 1){
			//load operation
			uint64_t xt;
			for (int i = 0; i < 7; i++){
				xt |= ((uint64_t) memory[addr + i]) << (8*i);
			}
			writeRegister(rt, xt, sf);
		}else{
			//store operation
			uint64_t xt = (uint64_t) readRegister(rt, sf);
			for (int i = 0; i < 7; i++) {
				memory[addr + i] = extractBits(xt, 8*i + 7,8*i);
			}
		}
	}

	if (indexFlag){
		writeRegister(xn, val, sf);
	}
}

void LL(uint32_t instruction, uint32_t *memory, uint32_t currAddress) {
	int sf = extractBits(instruction, 30,30);
	int simm = extractBits(instruction, 5, 23);
	int rt = extractBits(instruction, 0, 4);
	int64_t offset = simm * 4;
	writeRegister(rt, memory[currAddress + offset], sf);
}