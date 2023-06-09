#ifndef DEFINITION_H
#define DEFINITION_H

#include <stddef.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>


typedef struct {
    char *label;
    uint32_t address;
} row; 

#define MAX_LINE_LENGTH 256
#define MAX_TOKEN 6
#define delimiter ", #"

#endif
