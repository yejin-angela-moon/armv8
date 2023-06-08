#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "definitions.h"
#include "utilities.h"

char** readFile(int lineNum, int *countLabel ,char *filename) {
    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        fprintf(stderr, "can't open %s\n", filename);
        exit(1);
    }

    char buffer[MAX_LINE_LENGTH];
    char **strings = calloc(lineNum, sizeof(char*));

    for (int i = 0; i < lineNum; i++){
        if (fgets(buffer, sizeof(buffer), fp) != NULL)
        {
            if(isspace(buffer[0])){
                continue;
            }
            if (containColon(buffer[0])){
                countLabel++;
            }

            buffer[strcspn(buffer, "\n")] ='\0';
            strings[i] = (char*)calloc(MAX_LINE_LENGTH, sizeof(char));

            strcpy(strings[i], buffer);
        }  
    }
        
    fclose(fp);
}

void makeSymbolArray(row *table, int lineNum, char **lines){
    for (int i = 0; i < lineNum; i++){
        if (containColon(lines[i])){
            table[i].address = i * 4;
            table[i].label = lines[i];
            i--;
        }
    }
}


// void printStateToFile(state *state, char *filename) {

// }

// void printToString(state *state) {

//   //print registers
//   printf("Register:\n");
//   for (int i = 0; i < NUM_REGISTERS; i++) {
//     if (i < 10) {
//       printf("X0%d = %016lx\n", i, readRegister(i, 1, state->generalRegisters));
//     } else {
//       printf("X%d = %016lx\n", i, readRegister(i, 1, state->generalRegisters));
//     }
//   }

//   printf("PC = %08x\n", state->currAddress);
//   printf("PSTATE : %s%s%s%s\n",
//          state->pstate.N ? "N" : "-",
//          state->pstate.Z ? "Z" : "-",
//          state->pstate.C ? "C" : "-",
//          state->pstate.V ? "V" : "-");

//   //print non-zero memory
//   printf("Non-zero memory:\n");
//   for (int i = 0; i < MEMORY_SIZE; i++) {
//     if (state->memory[i] != 0) {
//       printf("0x%08x: %08x\n", i * 4, state->memory[i]);
//     }
//   }
// }