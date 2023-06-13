#include "utilities.h"

int count_lines(char *inputFile){
  FILE *fp = fopen(inputFile, "r");

  if (fp == NULL) {
    fprintf(stderr, "Failed to open the file.\n");
    exit(1);
  }

  int lineCount = 0;
  int character;

  while ((character = fgetc(fp)) != EOF) {
    if (character == '\n'){
      lineCount++;
    }
  }
  return lineCount;
}

bool containColon(char* line) {
  return(strchr(line, ':') != NULL);
}


char **tokenizer(char *line, int *numToken, char ** token) {
  //char string[20];
  printf("string %s\n", line);

  int m = 0;
  int k = 0;
  for (int c = 0; line[c]; c++) {
  //printf("c %d\n", c);
  if (line[c] == ' ') {
    //printf("enter if");
    token[m][k] = '\0';
    //printf("string %s\n", token[m]);
    //strcpy(token[m], string);
    //printf("mid token %s\n", token[0]);
    //(*token)++;
    k = 0;
    m++;
  } else if (line[c] == '#' || line[c] == ',') {

  } else {
   // printf("enter else");
    token[m][k] = line[c];
    //printf("leavibg else");
    k++;
  }

}
//printf("string %s\n", string);
  token[m][k] = '\0';
  *numToken = m;
//printf("string %s\n", string);
//printf("m %d\n", m);
//strcpy(token[m], string);
//printf("m %d\n", m);
//  printf("allocate mem\n");
  printf("token %s + %s + %s + %s\n ", token[0], token[1], token[2], token[3]);
  return token;
}


void freeLines(char **lines, int numLines){
  for (int i = 0; i < numLines; i++) {
    free(lines[i]);
  }
  free(lines);
}

/*void call_function(char* name, func_map* function_table, int numOfFuncs, char* tokens[], int numOfTokens) {
  for (int i = 0; i < numOfFuncs/sizeof(function_table[0]); i++) {
    if (strcmp(function_table[i].name, name) == 0) {
      function_table[i].func(tokens, numOfTokens);
      return;
    }
  }
  fprintf(stderr, "Function not found\n");
}*/

bool isStringInSet(char *target, char *set[], size_t setSize) {
  for (size_t i = 0; i < setSize; i++) {
    if (strcmp(target, set[i]) == 0) {
      return true; // found the string in the set
    }
  }
  return false; // the string was not found in the set
}

bool isRegister(const char* reg) {
  return (reg[0] == 'w' || reg[0] == 'x');
}

char* decToBinary(uint32_t x, int nbits) {
  char* res = (char *) malloc(32 * sizeof(char));
  assert(res != NULL);
  uint32_t mask = 1 << (nbits - 1);
  if ((x & mask) == 0) {
        strcpy(res, "0");
      } else {
        strcpy(res, "1");
      }
  mask = mask >> 1;
  for (int i = 1; i < nbits; i++) {
    if ((x & mask) == 0) {
      strcat(res, "0");
    } else {
      strcat(res, "1");
    }
    mask = mask >> 1;
  }
  strcat(res, "\0");
  return res;
}

uint32_t stringToNumber(char* string) {
  return (uint32_t)strtol(string, NULL, 0);
}


char* stringToBinary(char* string, int nbits) {
  return decToBinary(stringToNumber(string), nbits);
}

char* registerToBinary(char* reg) {
  // ex: "x11" -> "1011"
  assert(isRegister(reg));
  if (strcmp(reg + 1, "zr") == 0) {
    return "11111";
  }
  return decToBinary(stringToNumber(reg + 1), 5);
}

char* getSF(const char* reg) {
  assert(isRegister(reg));
  return reg[0] == 'w' ? "0" : "1";
}

int getNum(char *string, int start, int size) {
  char substring[size];
  strncpy(substring, string + start, size);
//  substring[--size];

  return (int) stringToNumber(substring);
}

uint32_t findAddressTable(char *label, row *table) {
  int i = 0;
  while(table[i].label[0] == '\0'){
    if (strcmp(table[i].label, label) == 0){
      return table[i].address;
    }
    i++;
  }
  return i;
}
