#include "iq_fit.h"

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

bool solvePuzzle(int board[BOARD_SIZE][BOARD_SIZE], PuzzlePiece pieces[], int index, int* solutionCount, int possible_board[MAX_SOLUTIONS][BOARD_SIZE][BOARD_SIZE]) {
  if (index == NUM_PIECES) {
    if (isBoardFull(board)) {
      if (*solutionCount < MAX_SOLUTIONS) {
        duplicateBoard(board, possible_board[*solutionCount]);
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
      if (pieces[index].empty < 0) {
        for (int rt = 0; rt < 2; rt++) {
          if (canPlacePiece(board, pieces[index], row, col, rt)) {
            placePiece(board, pieces[index], row, col, index + 1, rt);
            if (solvePuzzle(board, pieces, index + 1, solutionCount, possible_board)) {
              return true;
            }
            removePiece(board, pieces[index], row, col, rt);
          }
          if (pieces[index].height == pieces[index]. width) {
            break;
          }
        }
      } else {
        for (int rt = 0; rt < 4; rt++) {
          if (canPlacePiece(board, pieces[index], row, col, rt)) {
            placePiece(board, pieces[index], row, col, index + 1, rt);
            if (solvePuzzle(board, pieces, index + 1, solutionCount, possible_board)) {
              return true;
            }
            removePiece(board, pieces[index], row, col, rt);
          }
        }
      }
    }
  }
  return false;
}

void setUpGame(int question[BOARD_SIZE][BOARD_SIZE], char missing[],  int noOfRemove) {
  int count = 0;
  while (count != noOfRemove) {
    int r = rand() % NUM_PIECES + 1;
    while (foundEleChar(missing, switchToChar(r), count)) {
    }
    removePieceSec(question, r);
    missing[count] = switchToChar(r);
    count++;
  }
}

void displayBoard(int board[BOARD_SIZE][BOARD_SIZE], SDL_Renderer* renderer) {
  int curr_x = X_POS;
  int curr_y = Y_POS;

  SDL_Rect piece;
  piece.h = PIECE_SIZE;
  piece.w = PIECE_SIZE;

  int borderWidth = 1; // Border thickness
  SDL_Rect borderRect;
  borderRect.w = piece.w + 2 * borderWidth;
  borderRect.h = piece.h + 2 * borderWidth;

  for (int row = 0; row < BOARD_SIZE; row++) {
    for (int col = 0; col < BOARD_SIZE; col++) {
      piece.x = curr_x;
      piece.y = curr_y;
      borderRect.x = piece.x - borderWidth;
      borderRect.y = piece.y - borderWidth;

      // Render border
      SDL_SetRenderDrawColor(renderer, 74, 70, 55, SDL_ALPHA_OPAQUE); // Border color
      SDL_RenderDrawRect(renderer, &borderRect);

      // Render piece
      if (switchToChar(board[row][col]) == ' ') {
        SDL_SetRenderDrawColor(renderer, 92, 86, 62, SDL_ALPHA_OPAQUE); // Empty space color
      } else {
        SDL_SetRenderDrawColor(renderer, 201, 185, 119, SDL_ALPHA_OPAQUE); // Piece color
      }
      SDL_RenderFillRect(renderer, &piece);

      curr_x += PIECE_SIZE;
    }
    curr_x = X_POS;
    curr_y += PIECE_SIZE;
  }

  // Update the screen once per frame
  SDL_RenderPresent(renderer);

  SDL_Delay(1000);

  SDL_RenderClear(renderer);
}

void printPiece(PuzzlePiece piece, char name) {
  int count = 0;
  for (int i = 0; i < piece.height; i++) {
    for (int j = 0; j < piece.width; j++) {
      if (count == piece.empty) {
        printf("  ");
        count++;
        continue;
      }
      printf("%c ", name);
      count++;
    }
    printf("\n");
  }
  printf("\n");
}

int main() {
  // Set up the game
  srand(time(NULL));
  int board[BOARD_SIZE][BOARD_SIZE] = {0};
  PuzzlePiece pieces[NUM_PIECES] = {{2, 3, 4}, {1, 4, -1}, {3, 2, -1}, {1, 1, -1}, {2, 2, -1}, {5, 1, -1}, {4, 2, -1}, {2, 2, 1}};

  int solutionCount = 0;
  int possible_board[MAX_SOLUTIONS][BOARD_SIZE][BOARD_SIZE];

  printf("Looking for all possible solutions...\n");
  if (solvePuzzle(board, pieces, 0, &solutionCount, possible_board)) {
    printf("Maximum number of solutions reached.\n");
  } else {
    printf("%d solutions were found.\n", solutionCount);
  }

  int question[BOARD_SIZE][BOARD_SIZE];
  randomlyPickBoard(possible_board, question, solutionCount);

  int hp = 3;

  printBoard(question);

  printf("You can choose to remove 1 or 2 pieces from the board.\nThe pieces to be removed are random.\n");

  int no_missing;

  do {
    printf("Do you want to remove 1 piece or 2 pieces?\n");
    scanf("%d", &no_missing);
  } while (no_missing != 1 && no_missing != 2);

  printf("You chose to remove %d pieces\n", no_missing);

  char missing[NUM_PIECES];

  setUpGame(question, missing, no_missing);

  char added[NUM_PIECES + 1];
  int no_added = 0;

  printBoard(question);

  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    fprintf(stderr, "Could not initialize SDL: %s\n", SDL_GetError());
    return 1;
  }

  // Create an SDL window
  SDL_Window *window = SDL_CreateWindow(
          "Fill the board",
          SDL_WINDOWPOS_CENTERED,
          SDL_WINDOWPOS_CENTERED,
          WINDOW_WIDTH,
          WINDOW_HEIGHT,
          SDL_WINDOW_SHOWN
  );
  if (window == NULL) {
    fprintf(stderr, "Could not create window: %s\n", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  // Create a renderer
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == NULL) {
    fprintf(stderr, "Could not create renderer: %s\n", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  displayBoard(question, renderer);

  while (!isBoardFull(question) && hp > 0) {

    printf("The following pieces are missing: \n\n");
    for (int i = 0; i < no_missing; i++) {
      printPiece(pieces[missing[i] - 'A'], missing[i]);
    }

    int inputRow, inputCol, inputRt;
    char inputPiece;
    int scanResult;

    printf("Type: <Column> <Row> <Piece name> <Rotation> \n");

    scanResult = scanf("%d %d %c %d", &inputCol, &inputRow, &inputPiece, &inputRt);

    // Checking if 4 items were successfully read
    while (scanResult != 4) {
      printf("Invalid input format. Please type: <Column> <Row> <Piece name> <Rotation>\n");
      scanResult = scanf("%d %d %c %d", &inputCol, &inputRow, &inputPiece, &inputRt);
    }

    if (inputRow == -1 && foundEleChar(added, inputPiece, no_added)) {
      removePieceSec(question, inputPiece - 'A' + 1);
      printf("The piece has been removed\nHP: %d\n", hp);
      printBoard(question);
      displayBoard(question, renderer);
      missing[no_missing] = inputPiece;
      no_missing++;
      removeEle(added, inputPiece, &no_added);
      no_added--;
    } else if (inputCol < 0 || inputRow < 0 || inputRt < 0 || inputPiece - 'A' < 0 || inputPiece - 'A' > NUM_PIECES - 1) {
      hp--;
      printf("Invalid input.\nHP: %d\n", hp);
      printf("The board\n");
      printBoard(question);
      displayBoard(question, renderer);
    } else if (canPlacePiece(question, pieces[inputPiece - 'A'], inputRow, inputCol, inputRt) && foundEleChar(missing, inputPiece, no_missing)) {
      printf("Well done! You successfully added piece %c\n", inputPiece);
      placePiece(question, pieces[inputPiece - 'A'], inputRow, inputCol, inputPiece - 'A' + 1, inputRt);
      removeEle(missing, inputPiece, &no_missing);
      no_missing--;
      added[no_added] = inputPiece;
      no_added++;
      printf("The new board is: \nHP: %d\n", hp);
      printBoard(question);
      displayBoard(question, renderer);
    } else {
      hp--;
      printf("Invalid input.\nHP: %d\n", hp);
      printf("The board\n");
      printBoard(question);
      displayBoard(question, renderer);
    }
  }
  if (hp == 0) {
    printf("Game over - You've used up all your hp\n");
    return 0;
  }
  printf("You won - You filled in all the empty spaces!\n");

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}

