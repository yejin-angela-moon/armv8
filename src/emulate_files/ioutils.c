#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "definitions.h"
#include "utilities.h"

void readFile(state* state, char* filename){
  FILE *fp = fopen(filename, "rb");
  int fileSize;

  if (fp == NULL){
    fprintf(stderr, "can't open %s/n", filename);
    exit(1);
  }

  fseek(fp, 0, SEEK_END);
  fileSize = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  int numInstruction = fileSize/4;

  for (int i=0 ; i < numInstruction; i++){
    fread(&state->memory[i], sizeof(uint32_t), 1, fp);
  }

  // fread(state->memory, fileSize, 1, fp);

  fclose(fp);
}

void printStateToFile(state* state, char* filename){
  FILE *outputFile = fopen(filename, "w");

  if (outputFile == NULL){
    printf("Error opening file\n");
    exit(1);
  }

  //print registers
  fprintf(outputFile, "Registers:\n");
  for(int i = 0; i < NUM_REGISTERS; i++ ){
    fprintf(outputFile, "X%02d%*s = %016lx\n", i, 2, "", readRegister(i, 1, state->generalRegisters));
  }

  //Print PC
  fprintf(outputFile, "PC%*s = %016x\n", 4, "", state->currAddress);
  fprintf(outputFile, "PSTATE : %s%s%s%s\n",
          state->pstate.N ? "N" : "-",
          state->pstate.Z ? "Z" : "-",
          state->pstate.C ? "C" : "-",
          state->pstate.V ? "V" : "-");

  //print non-zero memory
  fprintf(outputFile, "Non-Zero Memory:\n");
  for (int i = 0; i < MEMORY_SIZE; i ++) {
    if (state->memory[i] != 0) {
      fprintf(outputFile, "0x%08x : %08x\n", i * 4, state->memory[i]);
    }
  }

  fclose(outputFile);
}

void printToString(state* state){

  //print registers
  printf("Registers:\n");
  for(int i = 0; i < NUM_REGISTERS; i++ ){
    printf("X%02d%*s = %016lx\n", i, 2, "", readRegister(i, 1, state->generalRegisters));
  }

  //Print PC
  printf("PC%*s = %016x\n", 4, "", state->currAddress);
  printf("PSTATE : %s%s%s%s\n",
         state->pstate.N ? "N" : "-",
         state->pstate.Z ? "Z" : "-",
         state->pstate.C ? "C" : "-",
         state->pstate.V ? "V" : "-");

  //print non-zero memory
  printf("Non-Zero Memory:\n");
  for (int i = 0; i < MEMORY_SIZE; i ++) {
    if (state->memory[i] != 0) {
      printf("0x%08x : %08x\n", i * 4, state->memory[i]);
    }
  }
}


