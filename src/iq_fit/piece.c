#include "piece.h"

bool canPlacePiece(int board[BOARD_SIZE][BOARD_SIZE], PuzzlePiece piece, int row, int col, int rt) {
  int width = rt % 2 == 0? piece.width : piece.height;
  int height = rt % 2 == 0? piece.height : piece.width;
  if (row < 0 || row + height > BOARD_SIZE || col < 0 || col + width > BOARD_SIZE || rt < 0 || rt > 3) {
    return false;
  }
  int count = 0;
  int empty = modifyEmpty(piece, rt);

  for (int i = row; i < row + height; i++) {
    for (int j = col; j < col + width; j++) {
      if (board[i][j] != 0 && count != empty) {
        return false;
      }
      count++;
    }
  }
  return true;
}

void placePiece(int board[BOARD_SIZE][BOARD_SIZE], PuzzlePiece piece, int row, int col, int number, int rt) {
  int count = 0;
  int empty = modifyEmpty(piece, rt);
  int width = rt % 2 == 0? piece.width : piece.height;
  int height = rt % 2 == 0? piece.height : piece.width;
  for (int i = row; i < row + height; i++) {
    for (int j = col; j < col + width; j++) {
      if (count == empty) {
        count++;
        continue;
      }
      board[i][j] = number;
      count++;
    }
  }
}

void removePiece(int board[BOARD_SIZE][BOARD_SIZE], PuzzlePiece piece, int row, int col, int rt) {
  int count = 0;
  int empty = modifyEmpty(piece, rt);
  int width = rt % 2 == 0? piece.width : piece.height;
  int height = rt % 2 == 0? piece.height : piece.width;
  for (int i = row; i < row + height; i++) {
    for (int j = col; j < col + width; j++) {
      if (count == empty) {
        count++;
        continue;
      }
      board[i][j] = 0;
      count++;
    }
  }
}

void removePieceSec(int board[BOARD_SIZE][BOARD_SIZE], int piece) {
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      if (board[i][j] == piece) {
        board[i][j] = 0;
      }
    }
  }
}

int modifyEmpty(PuzzlePiece piece, int rt) {
  if (piece.empty < 0 || rt == 0) {
    return piece.empty;
  }
  //printf("piece empty = %d\n", piece.empty);
  int pos_x = piece.empty % piece.width;
  int pos_y = piece.empty / piece.width;
  // printf("piece x = %d\n", pos_x);
  //printf("piece y = %d\n", pos_y);

  //  printf("piece y = %d\n", pos_y);
  int store_pos;
  if (rt > 0) {
    store_pos = pos_x;
    pos_x = pos_y;
    pos_y = piece.width - 1 - store_pos;
  }
  if (rt > 1) {
    store_pos = pos_x;
    pos_x = pos_y;
    pos_y = piece.height - 1 - store_pos;
  }
  if (rt > 2) {
    store_pos = pos_x;
    pos_x = pos_y;
    pos_y = piece.width - 1 - store_pos;
  }

  int no_row = rt % 2 == 0? piece.width : piece.height;
  // printf("after x = %d\n", pos_x);
  //printf("after y = %d\n", pos_y);
  //printf("row = %d\n", no_row);
  //printf("rt 1 empty = %d\n", pos_y * no_row + pos_x);
  return pos_y * no_row + pos_x;
}

