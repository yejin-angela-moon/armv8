#ifndef B_H
#define B_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "utilities.h"

static uint32_t conditional(char **token, int cond);
uint32_t B(row *table, char **token);

#endif