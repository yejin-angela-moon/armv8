#include "SDT.h"
#include "definition.h"

static int ldrInstruction(char **token) {
 uint32_t instruction = 0;
 if (token[1][0] == 'x'){
       instruction |= 1 << 30;
     }

     int rt = getNum(token[1], 1, 2);
     instruction |= rt;

     instruction |= SDT_INSTRUC;


       if (strcmp(token[0], "ldr") == 0){
         //ldr
         instruction |= 1 << 22;
       }
       int xn = getNum(token[2], 2, 2);
         instruction |= xn << 5;

         return instruction;
}

static uint32_t preIndexed(char **token){

  uint32_t instruction = 0;
  instruction  |= ldrInstruction(token);

  instruction |= 1 << 10;
  instruction |= 1 << 11;

  int32_t simm = stringToNumber(token[3]);
  if (strchr( token[3],'-') != NULL) {
     simm = (~simm) + 1;
  }


  instruction |= simm << 12;
  return instruction;
}

static uint32_t postIndexed(char **token, int countToken){
uint32_t instruction = 0;
 instruction  |= ldrInstruction(token);
  instruction |= 1 << 10;

  int16_t simm;
  if (countToken == 3){
    simm = 0;
  } else {
    simm  = strtol(strtok(token[3], "-"), NULL, 10);
    if (strchr( token[3],'-') != NULL) {
      simm = (~simm) + 1;
    }
  }

  instruction |= (simm & 0x01FF) << 12;
  return instruction;
}

static uint32_t registerOffset(char **token){

  uint32_t instruction = 0;
  instruction  |= ldrInstruction(token);


  instruction |= REGISTER_OFFSET << 10;

  int xm = getNum(token[3], 1, 2);
  instruction |= xm << 16;
  return instruction;
}

static uint32_t unsignedOffset(char **token, int countToken){
  uint32_t instruction = 0;

  instruction  |= ldrInstruction(token);

  instruction |= 1 << 24;

  if (countToken > 3){
    if(token[1][0] == 'x'){
    	instruction |= stringToNumber(token[3])/8 << 10;
    }else {
        instruction |= stringToNumber(token[3])/4 << 10;
    }
  }

  return instruction;
}

static uint32_t LL(char **token, row *table, uint32_t currAddress){
  uint32_t instruction = 0;
    if (token[1][0] == 'x'){
      instruction |= 1 << 30;
    }

    int rt = getNum(token[1], 1, 2);
    instruction |= rt;


  instruction |= LL_INSTRUC;

  uint32_t addressLabel = findAddressTable(token[2], table);

  int32_t offset = (addressLabel - currAddress) / 4;
  instruction |= (offset & MASK_TO_19BITS) << 5;

  return instruction;
}

uint32_t mode(char **token, int countToken){



  if (countToken >= 4 && strchr(token[3],'!') != NULL){
    return preIndexed(token);
  } else if(countToken >= 4  && strchr( token[2],']') != NULL){
   //printf("post\n  ");
    return postIndexed(token, countToken);
  } else if(countToken >= 4 && isRegister(strtok(token[3], "]"))){
   //printf("reg\n  ");
    return registerOffset(token);
  } else {
   //printf("unsigned\n  ");
    return unsignedOffset(token, countToken);
  }
}

uint32_t SDT(char **token, row *table, int countToken, uint32_t currAddress) {



  if (strchr(token[2],'[') == NULL){
    return LL(token, table, currAddress);
  }



  return mode(token, countToken);
}

