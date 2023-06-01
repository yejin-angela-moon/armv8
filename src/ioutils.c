#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "definition.h"
// #include "emulate.h"

#define HALT_INSTRUCTION 0x8a000000
#define MEMORY_SIZE (2 * 1024 * 1024)  // 2 MiB

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

	fread(memory, fileSize, 1, fp);

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
