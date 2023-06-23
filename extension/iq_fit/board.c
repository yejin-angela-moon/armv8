#include "board.h"

void duplicateBoard(int from[BOARD_SIZE][BOARD_SIZE], int to[BOARD_SIZE][BOARD_SIZE]) {
  for (int row = 0; row < BOARD_SIZE; row++) {
    for (int col = 0; col < BOARD_SIZE; col++) {
      to[row][col] = from[row][col];
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
  printf("Board %d chosen\n", r);
  duplicateBoard(possible_board[r], question);
}

