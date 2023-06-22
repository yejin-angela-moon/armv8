#include "board.h"

void duplicateBoard(int source[BOARD_SIZE][BOARD_SIZE], int destination[BOARD_SIZE][BOARD_SIZE]) {
  for (int row = 0; row < BOARD_SIZE; row++) {
    for (int col = 0; col < BOARD_SIZE; col++) {
      destination[row][col] = source[row][col];
    }
  }
}

bool isBoardFull(int board[BOARD_SIZE][BOARD_SIZE]) {
  for (int row = 0; row < BOARD_SIZE; row++) {
    for (int col = 0; col < BOARD_SIZE; col++) {
      if (board[row][col] == 0)
        return false;
    }
  }
  return true;
}

void randomlyPickBoard(int possible_board[MAX_SOLUTIONS][BOARD_SIZE][BOARD_SIZE], int question[BOARD_SIZE][BOARD_SIZE], int noOfSolution) {
  int r = rand() % noOfSolution;
  printf("Board %d is:  \n", r);
  duplicateBoard(possible_board[r], question);
}
