#ifndef EXTENSION_BOARD_H
#define EXTENSION_BOARD_H

#include "definitions.h"

void duplicateBoard(int from[BOARD_SIZE][BOARD_SIZE], int to[BOARD_SIZE][BOARD_SIZE]);

bool isBoardFull(int board[BOARD_SIZE][BOARD_SIZE]);

void randomlyPickBoard(int possible_board[MAX_SOLUTIONS][BOARD_SIZE][BOARD_SIZE], int question[BOARD_SIZE][BOARD_SIZE], int noOfSolution);

#endif //EXTENSION_BOARD_H
