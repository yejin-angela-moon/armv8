#include "utilities.h"

int count_lines(char *inputFile) {
  FILE *file;
  int ch;
  int linesCount = 0;
  bool isLineEmpty = 1;

  file = fopen(inputFile, "r");
  assert(file != NULL);

  while ((ch = fgetc(file)) != EOF) {
    if (ch == '\n') {
      if (!isLineEmpty) {
        linesCount++;
      }
      isLineEmpty = 1;
    } else {
      isLineEmpty = 0;
    }
  }

  fclose(file);
  return linesCount;
}

bool containColon(char *line) {
  return (strchr(line, ':') != NULL);
}

void deleteColon(char *line, unsigned long lineLength) {
  while (isspace(line[lineLength - 1]) || line[lineLength - 1] == ':') {
    line[lineLength - 1] = '\0';
    lineLength--;
  }
}

char **tokenizer(char *line, int *numToken, char **tokens) {
  int i = 0;
  //char **tokens = malloc(MAX_TOKEN * sizeof(char*));
  tokens[0] = strtok(line, delimiter);
  while (tokens[i] != NULL) {
    i++;
    tokens[i] = strtok(NULL, delimiter);
  }
  *numToken = i;
  return tokens;
}

void freeLines(char **lines, int numLines) {
  for (int i = 0; i < numLines; i++) {
    free(lines[i]);
  }
  free(lines);
}

bool isStringInSet(char *target, char *set[], size_t setSize) {
  for (size_t i = 0; i < setSize; i++) {
    if (strcmp(target, set[i]) == 0) {
      return true; // found the string in the set
    }
  }
  return false; // the string was not found in the set
}

bool isRegister(const char *reg) {
  return (tolower(reg[0]) == 'w' || tolower(reg[0]) == 'x');
}

/*

char *decToBinary(uint32_t x, int nbits) { //no need for this now
  char *res = (char *) malloc(32 * sizeof(char));
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

uint32_t stringToNumber(char *string) {  //dont think we need to make one function for this
  return (uint32_t) strtol(string, NULL, 0);
}


char *stringToBinary(char *string, int nbits) { //dont think we need this now
  return decToBinary(stringToNumber(string), nbits);
}

uint32_t binaryStringToNumber(char* string) { //no need for this
  return (uint32_t) strtoll(string, NULL, 2);
}
*/
int registerToBinary(char *reg) {
  // ex: "x11" -> "1011"
  assert(isRegister(reg));
  if (strcmp(reg + 1, "zr") == 0) {
    return 0x1F;
  }
  return strtol((reg + 1), NULL, 0);
}

char *getSF(const char *reg) {
  assert(isRegister(reg));
  return tolower(reg[0] == 'w') ? "0" : "1";
}

int getNum(char *string, int start, int size) {
  char substring[size];
  strncpy(substring, string + start, size);
//  substring[--size];

  return strtol(substring, NULL, 0);
}

uint32_t findAddressTable(char *label, row *table) {
  int i = 0;
  while (table[i].label[0] != '\0') {
    if (strcmp(table[i].label, label) == 0) {
      return table[i].address;
    }
    i++;
  }
  return i;
}

char *getZeroRegister(const char *reg) {
  return tolower(reg[0]) == 'w' ? "wzr" : "xzr";
}
