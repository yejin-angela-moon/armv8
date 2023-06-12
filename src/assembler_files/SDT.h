#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "definition.h"
#include "utilities.h"

#define LL_TOKEN_NUM 3
#define REGISTER_OFFSET 0x81A
#define LL_INSTRUC 0x18000000
#define MASK_TO_19BITS 0x7FFFF
#define SDT_INSTRUC 0xB8000000

static u_int32_t preIndexed(char **token, uint32_t instruction);
static uint32_t postIndexed(char **token, uint32_t instruction);
static uint32_t registerOffset(char **token, uint32_t instruction);
static uint32_t unsignedOffset(char **token, uint32_t instruction);
static uint32_t LL(char **token, row *table, uint32_t instruction, uint32_t currAddress);
uint32_t mode(char **token, row *table, uint32_t instruction);
uint32_t SDT(char **token, row *table, int countToken, uint32_t currAddress);