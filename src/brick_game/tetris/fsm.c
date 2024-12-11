#include "fsm.h"

void handleTime() {
  GameState_t *state = getState();
  TimeInfo_t *time = getTime();
  clock_gettime(CLOCK_MONOTONIC, &time->end);
  GameInfo_t *gameInfo = getGameInfo();
  if (((time->end.tv_sec - time->start.tv_sec) +
       (time->end.tv_nsec - time->start.tv_nsec) / 1e9) >
      1.1 - (0.1 * gameInfo->speed)) {
    *state = SHIFTING;
    clock_gettime(CLOCK_MONOTONIC, &time->start);
  }
}

void userInput(UserAction_t action, bool hold) {
  (void)hold;
  GameState_t *state = getState();
  switch (*state) {
    case START:
      onStartState(action);
      break;
    case SPAWN:
      onSpawnState();
      break;
    case MOVING:
      onMovingState(action);
      break;
    case SHIFTING:
      onShiftingState();
      break;
    case COLLIDE:
      onCollideState();
      break;
    case GAMEOVER:
      onGameOverState();
      break;
    default:
      break;
  }
}

void onStartState(UserAction_t signal) {
  GameState_t *state = getState();
  GameInfo_t *gameInfo = getGameInfo();
  gameInfo->pause = 1;
  switch (signal) {
    case Start:
      *state = SPAWN;
      break;
    case Up:
      changeSpeed(1);
      break;
    case Down:
      changeSpeed(-1);
      break;
    case Terminate:
      *state = GAMEOVER;
      break;
    default:
      *state = START;
      break;
  }
}

void onSpawnState() {
  GameState_t *state = getState();
  TetrominoInfo_t *tetromino = getTetromino();
  GameInfo_t *gameInfo = getGameInfo();
  gameInfo->pause = 0;
  tetromino->x = 3;
  tetromino->y = findBottomBlockPos(tetromino->map);
  int **newTetromino = getRandomFigure();
  copyFigureFromLeftToRight(gameInfo->next, tetromino->map);
  copyFigureFromLeftToRight(newTetromino, gameInfo->next);
  free(newTetromino);
  *state = MOVING;
}

void onMovingState(UserAction_t signal) {
  GameState_t *state = getState();
  GameInfo_t *gameInfo = getGameInfo();
  int pause = gameInfo->pause;
  switch (signal) {
    case Action:
      if (!isFigureSquare() && !pause) rotate();
      break;
    case Down:
#ifdef CANON
      if (!isCollide() && !pause) moveDown();
#else
      while (!isCollide() && !pause) moveDown();
      *state = COLLIDE;
#endif
      break;
    case Right:
      if (!pause) moveRight();
      break;
    case Left:
      if (!pause) moveLeft();
      break;
    case Pause:
      gameInfo->pause = (gameInfo->pause == 1) ? 0 : 1;
      break;
    case Terminate:
      *state = GAMEOVER;
      break;
    default:
      break;
  }
  if (!pause && signal != Terminate) handleTime();
}

void onShiftingState() {
  GameState_t *state = getState();
  if (isCollide())
    *state = COLLIDE;
  else {
    moveDown();
    *state = MOVING;
  }
}

void onCollideState() {
  GameState_t *state = getState();
  handleFilledRows();
  handleLevel();
  if (isFieldFull())
    *state = GAMEOVER;
  else
    *state = SPAWN;
}

void onGameOverState() {
  TetrominoInfo_t *tetromino = getTetromino();
  GameInfo_t *gameInfo = getGameInfo();
  free(tetromino->map);
  free(gameInfo->field);
  free(gameInfo->next);
  gameInfo->pause = 2;
}