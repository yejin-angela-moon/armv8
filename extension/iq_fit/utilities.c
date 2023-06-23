#include "utilities.h"

char switchToChar(int index) {
  if (index <= NUM_PIECES && index >= 1) {
    return index + 'A' - 1;
  }
  return ' ';
}

bool foundEleChar(char array[], char letter, int size) {
  for (int i = 0; i < size; i++) {
    if (array[i] == letter) {
      return true;
    }
  }
  return false;
}

void removeEle(char array[], char letter, int size) {
  bool replace = false;
  for (int i = 0; i < size - 1; i++) {
    if (array[i] == letter) {
      replace = true;
    }
    if (replace) {
      array[i] = array[i + 1];
    }
  }
}

