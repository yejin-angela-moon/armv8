#ifndef IOUTILS_H
#define IOUTILS_H

char** readFile(int lineNum, int *countLabel ,char *filename);
void makeSymbolTable(row *table, int lineNum, char **lines);

#endif