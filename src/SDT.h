#ifndef  SDT_H
#define SDT_H

#include <stdint.h>

void SDT(uint32_t instruction, uint32_t* memory);
void LL(uint32_t instruction, uint32_t *memory, uint32_t currAddress);

#endif
