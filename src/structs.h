#ifndef STRUCTS_H
#define STRUCTS_H

#include <time.h>

typedef enum {
  START,
  SPAWN,
  MOVING,
  SHIFTING,
  COLLIDE,
  GAMEOVER,
  EXIT_STATE
} GameState_t;

typedef struct {
  struct timespec start;
  struct timespec end;
} TimeInfo_t;

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

typedef struct {
  int y;
  int x;
  int **map;
} TetrominoInfo_t;

#endif