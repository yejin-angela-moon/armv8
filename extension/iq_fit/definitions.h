//
// Created by ym1721 on 21/06/23.
//

#ifndef EXTENSION_DEFINITIONS_H
#define EXTENSION_DEFINITIONS_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define BOARD_SIZE 6
#define PIECE_SIZE 50
#define NUM_PIECES 8
#define MAX_SOLUTIONS 5000
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define X_POS 50
#define Y_POS 50

typedef struct {
    int width;
    int height;
    int empty;
} PuzzlePiece;

#endif //EXTENSION_DEFINITIONS_H
