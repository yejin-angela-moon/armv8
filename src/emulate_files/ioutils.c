#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "definitions.h"
#include "utilities.h"

#define HALT_INSTRUCTION 0x8a000000
#define MEMORY_SIZE (2 * 1024 * 1024)  // 2 MiB

void readFile(state* state, char* filename){
	FILE *fp = fopen(filename, "rb");
	int fileSize;
	
	if (fp == NULL){
		fprintf(stderr, "can't opern %s/n", filename);
		exit(1);
	}

	fseek(fp, 0, SEEK_END);
	fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	int numInstruction = fileSize/sizeof(uint32_t);
	
	for (int i=0 ; i < numInstruction; i++){
		fread(&state->memory[i], sizeof(uint32_t), 1, fp);
	}
	
	// fread(state->memory, fileSize, 1, fp);

	fclose(fp);
}

void printStateToFile(state* state, char* filename){
	FILE *outputFile = fopen(filename, "w");

	if (outputFile == NULL){
		printf("Error opeing file\n");
		exit(1);
	}

	//print registers
	fprintf(outputFile, "Register:\n");
	for(int i = 0; i < NUM_REGISTERS; i++ ){
		if (i < 10) {
            fprintf(outputFile, "X0%d = %016lx\n", i, readRegister(i, 0, state->generalRegisters));
        } else {
            fprintf(outputFile, "X%d = %016lx\n", i, readRegister(i, 0, state->generalRegisters));
        }
	} 

	//Print PC
	fprintf(outputFile, "PC = %08x\n", state->currAddress);
	fprintf(outputFile, "PSTATE : %s%s%s%s\n", 
		state->pstate.N ? "N" : "-", 
		state->pstate.Z ? "Z" : "-", 
		state->pstate.C ? "C" : "-", 
		state->pstate.V ? "V" : "-");

	//print non-zero memory
	fprintf(outputFile, "Non-zero memory:\n");
	for (int i = 0; i < MEMORY_SIZE; i += 4) {
        if (state->memory[i] != 0) {
            fprintf(outputFile, "0x%08x: %08x\n", i * 4, state->memory[i]);
        }
    }

	fclose(outputFile);
}

void printToString(state* state){

	//print registers
	printf("Register:\n");
	for(int i = 0; i < NUM_REGISTERS; i++ ){
		if (i < 10) {
            printf("X0%d = %016lx\n", i, readRegister(i, 0, state->generalRegisters));
        } else {
            printf("X%d = %016lx\n", i, readRegister(i, 0, state->generalRegisters));
        }
	} 

	//Print PC
	printf("PC = %08x\n", state->currAddress);
	printf("PSTATE : %s%s%s%s\n", 
		state->pstate.N ? "N" : "-", 
		state->pstate.Z ? "Z" : "-", 
		state->pstate.C ? "C" : "-", 
		state->pstate.V ? "V" : "-");

	//print non-zero memory
	printf("Non-zero memory:\n");
	for (int i = 0; i < MEMORY_SIZE; i += 4) {
        if (state->memory[i] != 0) {
            printf("0x%08x: %08x\n", i, state->memory[i]);
        }
    }
}
