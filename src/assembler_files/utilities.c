#include "utilities.h"

// count the number of lines in the file
int count_lines(char *inputFile) {
  FILE *file;
  int ch;
  int linesCount = 0;
  bool isLineEmpty = true;

  file = fopen(inputFile, "r");
  assert(file != NULL);

  while ((ch = fgetc(file)) != EOF) {
    if (ch == '\n') {
      if (!isLineEmpty) {
        linesCount++;
      }
      isLineEmpty = true;
    } else {
      isLineEmpty = false;
    }
  }

  fclose(file);
  return linesCount;
}

// returns true if string contains a colon - indicates a label
bool containsColon(char *line) {
  return (strchr(line, ':') != NULL);
}

// deletes the colon from the string - to save label name only
void deleteColon(char *line, unsigned long lineLength) {
  while (isspace(line[lineLength - 1]) || line[lineLength - 1] == ':') {
    line[lineLength - 1] = '\0';
    lineLength--;
  }
}

// splits a string up into 'tokens', which are labels, instruction mnemonics, or imm/reg values
char **tokenizer(char *line, int *numToken, char **tokens) {
  int i = 0;
  tokens[0] = strtok(line, delimiter); // delimiters are comma, hashtag, tab and whitespace
  while (tokens[i] != NULL) {
    i++;
    tokens[i] = strtok(NULL, delimiter);
  }
  *numToken = i;
  return tokens;
}

// frees the space allocated for each line
void freeLines(char **lines, int numLines) {
  for (int i = 0; i < numLines; i++) {
    free(lines[i]);
  }
  free(lines);
}

// returns true if the target string is found in the given set of strings
bool isStringInSet(char *target, char *set[], size_t setSize) {
  for (size_t i = 0; i < setSize; i++) {
    if (strcmp(target, set[i]) == 0) {
      return true;
    }
  }
  return false;
}

// returns true if the string is identified as a register - if name starts with x or w
bool isRegister(const char *reg) {
  return (tolower(reg[0]) == 'w' || tolower(reg[0]) == 'x');
}

// extracts and returns the binary value for the register
int registerToBinary(char *reg) {
  // ex: "x11" -> 0b1011
  assert(isRegister(reg));
  if (strcmp(reg + 1, "zr") == 0) {
    return ZERO_REGISTER_VALUE; // return 11111 for zero register
  }
  return (int) strtol((reg + 1), NULL, 0);
}

// returns the sf value corresponding to the register type - 0 for w register
uint32_t getSF(const char *reg) {
  assert(isRegister(reg));
  return tolower(reg[0] == 'w') ? 0 : 1;
}

// converts part of the string (number written in string) into integer and returns it
int getSubstringAsInt(char *string, int start, int size) {
  char substring[size];
  strncpy(substring, string + start, size);
  return (int) strtol(substring, NULL, 0);
}

// returns the address of the target label in the symbol table
uint32_t findAddressOfLabel(char *label, symbol_table_row *symbol_table) {
  int i = 0;
  while (symbol_table[i].label[0] != '\0') {
    if (strcmp(symbol_table[i].label, label) == 0) {
      return symbol_table[i].address;
    }
    i++;
  }
  return i;
}

// returns the correct size zero integer
char *getZeroRegister(const char *reg) {
  return tolower(reg[0]) == 'w' ? "wzr" : "xzr";
}
