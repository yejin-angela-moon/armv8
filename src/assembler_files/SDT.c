#include "SDT.h"
#include "definition.h"

static uint32_t preIndexed(char **token) {

    uint32_t instruction = 0;
    if (token[1][0] == 'x') {
        instruction |= 1 << 30;
    }

    int rt = getSubstringAsInt(token[1], 1, 2);
    instruction |= rt;

    instruction |= SDT_INSTRUC;

    if (strcmp(token[0], "ldr") == 0) {
        //ldr
        instruction |= 1 << 22;
    }
    int xn = getSubstringAsInt(token[2], 2, 2);
    instruction |= xn << 5;

    instruction |= 1 << 10;
    instruction |= 1 << 11;

    int32_t simm = (int) strtol(token[3], NULL, 0);
    if (strchr(token[3], '-') != NULL) {
        simm = (~simm) + 1;
    }

    instruction |= simm << 12;
    return instruction;
}

static uint32_t postIndexed(char **token, int countToken) {
    uint32_t instruction = 0;
    if (token[1][0] == 'x') {
        instruction |= 1 << 30;
    }

    int rt = getSubstringAsInt(token[1], 1, 2);
    instruction |= rt;

    instruction |= SDT_INSTRUC;


    if (strcmp(token[0], "ldr") == 0) {
        instruction |= 1 << 22;
    }

    int xn = getSubstringAsInt(token[2], 2, 2);
    instruction |= xn << 5;
    instruction |= 1 << 10;

    int16_t simm;
    simm = (int16_t) strtol(strtok(token[3], "-"), NULL, 10);
    if (strchr(token[3], '-') != NULL) {
        simm = (int16_t) ((~simm) + 1);
    }

    instruction |= (simm & 0x01FF) << 12;
    return instruction;
}

static uint32_t registerOffset(char **token) {

    uint32_t instruction = 0;
    if (tolower(token[1][0]) == 'x') {
        instruction |= 1 << 30;
    }

    int rt = getSubstringAsInt(token[1], 1, 2);
    instruction |= rt;
    instruction |= SDT_INSTRUC;


    if (strcmp(token[0], "ldr") == 0) {
        instruction |= 1 << 22;
    }
    int xn = getSubstringAsInt(token[2], 2, 2);
    instruction |= xn << 5;

    instruction |= REGISTER_OFFSET << 10;

    int xm = getSubstringAsInt(token[3], 1, 2);
    instruction |= xm << 16;
    return instruction;
}

static uint32_t unsignedOffset(char **token, int countToken) {
    uint32_t instruction = 0;
    if (token[1][0] == 'x') {
        instruction |= 1 << 30;
    }

    int rt = getSubstringAsInt(token[1], 1, 2);
    instruction |= rt;

    instruction |= SDT_INSTRUC;


    if (strcmp(token[0], "ldr") == 0) {
        instruction |= 1 << 22;
    }
    int xn = getSubstringAsInt(token[2], 2, 2);
    instruction |= xn << 5;


    instruction |= 1 << 24;

    if (countToken > 3) {
        if (tolower(token[1][0]) == 'x') {
            instruction |= strtol(token[3], NULL, 0) / 8 << 10;
        } else {
            instruction |= strtol(token[3], NULL, 0) / 4 << 10;
        }
    }

    return instruction;
}

static uint32_t LL(char **token, symbol_table_row *table, uint32_t currAddress) {
    uint32_t instruction = 0;
    if (tolower(token[1][0]) == 'x') {
        instruction |= 1 << 30;
    }

    int rt = getSubstringAsInt(token[1], 1, 2);
    instruction |= rt;

    instruction |= LL_INSTRUC;

    uint32_t addressLabel = findAddressOfLabel(token[2], table);

    int32_t offset = (int) (addressLabel - currAddress) / 4;
    instruction |= (offset & MASK_TO_19BITS) << 5;

    return instruction;
}

uint32_t mode(char **token, int countToken) {
    if (countToken >= 4 && strchr(token[3], '!') != NULL) {
        return preIndexed(token);
    } else if (countToken >= 4 && strchr(token[2], ']') != NULL) {
        return postIndexed(token, countToken);
    } else if (countToken >= 4 && isRegister(strtok(token[3], "]"))) {
        return registerOffset(token);
    } else {
        return unsignedOffset(token, countToken);
    }
}

uint32_t SDT(char **token, symbol_table_row *table, int countToken, uint32_t currAddress) {
    if (strchr(token[2], '[') == NULL) {
        return LL(token, table, currAddress);
    }
    return mode(token, countToken);
}
