#include "utilities.h"
#include "ioutils.h"

void readFile(state* state, char* filename){
	FILE *fp = fopen(filename, "rb");
	long int fileSize;
	
	if (fp == NULL){
		fprintf(stderr, "can't open %s/n", filename);
		exit(1);
	}

	fseek(fp, 0, SEEK_END);
	fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	fread(state->memory, fileSize, 1, fp);

	fclose(fp);
}

void printStateToFile(state* state, char* filename){
	FILE *outputFile = fopen(filename, "w");

	if (outputFile == NULL){
		printf("Error opening file\n");
		exit(1);
	}

	//print registers
	fprintf(outputFile, "Register:\n");
	for(int i = 0; i < NUM_REGISTERS; i++ ){
		if (i < 10) {
            fprintf(outputFile, "X0%d = %016lx\n", i, readRegister(i, 1, state->generalRegisters));
        } else {
            fprintf(outputFile, "X%d = %016lx\n", i, readRegister(i, 1, state->generalRegisters));
        }
	} 

	//Print PC
	fprintf(outputFile, "PC = %016x\n", state->currAddress);
	fprintf(outputFile, "PSTATE : %s%s%s%s\n", 
		state->pstate.N ? "N" : "-", 
		state->pstate.Z ? "Z" : "-", 
		state->pstate.C ? "C" : "-", 
		state->pstate.V ? "V" : "-");

	//print non-zero memory
	fprintf(outputFile, "Non-zero memory:\n");
	for (int i = 0; i < MEMORY_SIZE; i++) {
        if (state->memory[i] != 0) {
            fprintf(outputFile, "0x%08x: %08x\n", i * 4, state->memory[i]);
        }
    }

	fclose(outputFile);
}

void printToString(state* state){

	//print registers
	printf("Register:\n");
	for(int i = 0; i < NUM_REGISTERS; i++){
		if (i < 10) {
            printf("X0%d = %016lx\n", i, readRegister(i, 1, state->generalRegisters));
        } else {
            printf("X%d = %016lx\n", i, readRegister(i, 1, state->generalRegisters));
        }
	} 

	//Print PC
	printf("PC = %016x\n", state->currAddress);
	printf("PSTATE : %s%s%s%s\n", 
		state->pstate.N ? "N" : "-", 
		state->pstate.Z ? "Z" : "-", 
		state->pstate.C ? "C" : "-", 
		state->pstate.V ? "V" : "-");

	//print non-zero memory
	printf("Non-zero memory:\n");
	for (int i = 0; i < MEMORY_SIZE; i++) {
        if (state->memory[i] != 0) {
            printf("0x%08x: %08x\n", i * 4, state->memory[i]);
        }
    }
}
