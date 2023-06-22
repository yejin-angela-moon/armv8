#ifndef EXTENSION_PIECE_H
#define EXTENSION_PIECE_H

#include "definitions.h"

bool canPlacePiece(int board[BOARD_SIZE][BOARD_SIZE], PuzzlePiece piece, int row, int col, int rt);

void placePiece(int board[BOARD_SIZE][BOARD_SIZE], PuzzlePiece piece, int row, int col, int number, int rt);

void removePiece(int board[BOARD_SIZE][BOARD_SIZE], PuzzlePiece piece, int row, int col, int rt);

void removePieceSec(int board[BOARD_SIZE][BOARD_SIZE], int piece);

int modifyEmpty(PuzzlePiece piece, int rt);

#endif //EXTENSION_PIECE_H
