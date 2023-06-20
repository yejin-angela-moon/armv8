#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>


#define BOARD_SIZE 6
#define NUM_PIECES 9
#define MAX_SOLUTIONS 1000

typedef struct {
    char name;
    int width;
    int height;
} PuzzlePiece;

enum PieceName {A, B, C, D, E, F, G, H};

void copyBoard(int source[BOARD_SIZE][BOARD_SIZE], int destination[BOARD_SIZE][BOARD_SIZE]) {
  for (int row = 0; row < BOARD_SIZE; row++) {
    for (int col = 0; col < BOARD_SIZE; col++) {
      destination[row][col] = source[row][col];
    }
  }
}

bool canPlacePiece(int board[BOARD_SIZE][BOARD_SIZE], PuzzlePiece piece, int row, int col) {
  if (row < 0 || row + piece.height > BOARD_SIZE || col < 0 || col + piece.width > BOARD_SIZE)
    return false;

  for (int i = row; i < row + piece.height; i++) {
    for (int j = col; j < col + piece.width; j++) {
      if (board[i][j] != 0)
        return false;
    }
  }

  return true;
}

void placePiece(int board[BOARD_SIZE][BOARD_SIZE], PuzzlePiece piece, int row, int col, int number) {
  for (int i = row; i < row + piece.height; i++) {
    for (int j = col; j < col + piece.width; j++) {
      board[i][j] = number;
    }
  }
}

void removePiece(int board[BOARD_SIZE][BOARD_SIZE], PuzzlePiece piece, int row, int col) {
  for (int i = row; i < row + piece.height; i++) {
    for (int j = col; j < col + piece.width; j++) {
      board[i][j] = 0;
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

char switchToChar(int index) {
  switch (index) {
    case 1:
      return 'A';
      break;
    case 2:
      return 'B';
      break;
    case 3:
      return 'C';
      break;
    case 4:
      return 'D';
      break;
    case 5:
      return 'E';
      break;
    case 6:
      return 'F';
      break;
    case 7:
      return 'G';
      break;
    case 8:
      return 'H';
      break;
    default:
      return ' ';
      break;
  }
}

void printBoard(int board[BOARD_SIZE][BOARD_SIZE]) {
  printf("  0 1 2 3 4 5\n");
  for (int row = 0; row < BOARD_SIZE; row++) {
    printf("%d ", row);
    for (int col = 0; col < BOARD_SIZE; col++) {
      printf("%c ", switchToChar(board[row][col]));
    }
    printf("\n");
  }
  printf("\n");
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

bool solvePuzzle(int board[BOARD_SIZE][BOARD_SIZE], PuzzlePiece pieces[], int index, int* solutionCount, int possible_board[MAX_SOLUTIONS][BOARD_SIZE][BOARD_SIZE]) {
  if (index >= NUM_PIECES) {
    if (isBoardFull(board)) {
      if (*solutionCount < MAX_SOLUTIONS) {
        copyBoard(board, possible_board[*solutionCount]);
        (*solutionCount)++;
      }

      if (*solutionCount == MAX_SOLUTIONS){
        return true;
      }
    }
    return false;
  }

  for (int row = 0; row < BOARD_SIZE; row++) {
    for (int col = 0; col < BOARD_SIZE; col++) {
      if (canPlacePiece(board, pieces[index], row, col)) {
        placePiece(board, pieces[index], row, col, index + 1);
        if (solvePuzzle(board, pieces, index + 1, solutionCount, possible_board)) {
          return true;
        }
        removePiece(board, pieces[index], row, col);
      }
    }
  }

  return false;
}


int** randomlyPickBoard(int possible_board[MAX_SOLUTIONS][BOARD_SIZE][BOARD_SIZE]) {
  int r = rand() % 288;
  printf("The %d board is:  \n", r);
  int **board = calloc(sizeof(int*), BOARD_SIZE);
  //  printf("try alloc");
  if (board == NULL) {
    printf("fail on alloc board");
  }
  for (int i = 0; i < BOARD_SIZE; i++) {
    board[i] = malloc(BOARD_SIZE * sizeof(int));
    if (board[i] == NULL) {
      printf("fail on alloc board [%d]", i);
    }
  }
  // printf("mem alloc");
  for (int row = 0; row < BOARD_SIZE; row++) {
    for (int col = 0; col < BOARD_SIZE; col++) {
      board[row][col] = possible_board[r][row][col];
    }
  }
  //board = possible_board[r];
  //  printf("return board");
  return board;
}

int main() {
  srand(time(NULL));
  int board[BOARD_SIZE][BOARD_SIZE] = {0};
  PuzzlePiece pieces[NUM_PIECES] = {{'A', 2, 3}, {'B', 1, 3}, {'C', 3, 2}, {'D', 4, 2}, {'E', 2, 2}, {'F', 1, 1}, {'G', 3, 1}, {'H', 1, 5}};

  int solutionCount = 0;
  int possible_board[MAX_SOLUTIONS][BOARD_SIZE][BOARD_SIZE];

  if (solvePuzzle(board, pieces, 0, &solutionCount, possible_board)) {
    printf("Maximum number of solutions reached.\n");
  } else {
    printf("Only %d solutions found.\n", solutionCount);
  }


  int question[BOARD_SIZE][BOARD_SIZE];
  int **store_question = randomlyPickBoard(possible_board);
  for (int row = 0; row < BOARD_SIZE; row++) {
    for (int col = 0; col < BOARD_SIZE; col++) {
      question[row][col] = store_question[row][col];
    }
  }

  //printBoard(question);
  int r = rand() % 7 + 1;
  removePieceSec(question, r);
  printf("Piece %c [%d, %d] \n", switchToChar(r), pieces[r - 1].width, pieces[r - 1].height);
  r = rand() % 7 + 1;
  removePieceSec(question, r);
  printf("Piece %c [%d, %d] \n", switchToChar(r), pieces[r - 1].width, pieces[r - 1].height);

  printBoard(question);

  while (!isBoardFull(question)) {

    int inputRow;
    int inputCol;
    char inputPiece;


    printf("Type a the row and the col and piece and press enter: \n");

    scanf("%d %d %c", &inputRow, &inputCol, &inputPiece);

    //enum PieceName inputName = A;

    //printf("The piece is %d\n", inputName);

    if (canPlacePiece(question, pieces[inputPiece - 'A'], inputRow, inputCol)) {
      printf("Valid\n");
      placePiece(question, pieces[inputPiece - 'A'], inputRow, inputCol, inputPiece - 'A' + 1);
      printf("The new board is: \n");
      printBoard(question);
    } else {
      printf("The piece can't be placed here\n");
    }
  }

  printf("You fill all the spaces!\n");



  return 0;
}
