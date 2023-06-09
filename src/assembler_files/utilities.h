
#include <stdbool.h>

int count_lines(char *inputFile);
bool containColon(char* line);
void freeLines(char **lines, int numLines);
char **tokenizer(char *line);
static char* decToBinary(uint32_t x, int nbits);
static uint32_t stringToNumber(char* string);
char* stringToBinary(char* string, int nbits);
bool isRegister(const char* reg);
char* registerToBinary(char* reg);
char* getSF(const char* reg);