#ifndef TESTS_H
#define TESTS_H

#include <check.h>
#include <math.h>

#include "../brick_game/tetris/backend.h"
#include "../brick_game/tetris/fsm.h"
#include "../structs.h"

#define TRUE 1
#define FALSE 0

Suite *fsm_test(void);

int isEqualFigure(int **matrix1, int **matrix2);

#endif
