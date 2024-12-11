#ifndef BACKEND_H
#define BACKEND_H

#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "fsm.h"
#include "tetris.h"

GameInfo_t *getGameInfo();
GameState_t *getState();
TimeInfo_t *getTime();
TetrominoInfo_t *getTetromino();

void initGame(GameInfo_t *gameInfo);
int **createMatrix(int rows, int columns);
int **allocBuffer(int rows, int column);

int randomTetromino();
int **getRandomFigure();
void copyFigureFromLeftToRight(int **source, int **destination);
void addTetrominoToField();
void eraseTetromino();

void handleFilledRows();
void handleLevel();
void shiftRowsDownFrom(int y);

void moveRight();
void moveLeft();
void moveDown();
void rotate();

int findLeftBlockPos(int **map);
int findRightBlockPos(int **map);
int findBottomBlockPos(int **map);
int findXCoordAfterRotate(int **map);

bool isCollide();
bool isFigureSquare();
bool isForbiddenMoveLeft(TetrominoInfo_t *tetromino);
bool isForbiddenMoveRight(TetrominoInfo_t *tetromino);
bool isFillRow(int y);
bool isAvailableToAdd(int tempX, int **rotated_temp);
bool isFieldFull();

int getHiScore();
void saveHighScore(int highScore);
void changeSpeed(int speed);

#endif