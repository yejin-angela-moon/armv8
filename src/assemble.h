#ifndef ARMV8_48_ASSEMBLE_H
#define ARMV8_48_ASSEMBLE_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "assembler_files/definitions.h"
#include "assembler_files/ioutils.h"
#include "assembler_files/utilities.h"
#include "assembler_files/DP.h"
#include "assembler_files/B.h"
#include "assembler_files/SDT.h"

char *dpSet[] = {"add", "adds", "sub", "subs", "movn", "movz",
                 "movk", "and", "bic", "orr", "orn", "eon",
                 "eor", "ands", "bics", "madd", "msub"};
// the collection of data processing instruction mnemonics
char *sdtSet[] = {"ldr", "str"};
// the collection of single data transfer instruction mnemonics
#define dpSetSize 17
#define sdtSetSize 2

#endif //ARMV8_48_ASSEMBLE_H
