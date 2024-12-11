#include <stdio.h>

#include "tests.h"

START_TEST(test_fsm_1) {
  GameState_t *state = getState();
  *state = START;
  UserAction_t action = Terminate;
  userInput(action, 1);
  ck_assert_int_eq(*state, GAMEOVER);
}
START_TEST(test_fsm_2) {
  GameState_t *state = getState();
  GameInfo_t *gameInfo = getGameInfo();
  gameInfo->speed = 2;
  *state = START;
  UserAction_t action = Down;
  userInput(action, 1);
  ck_assert_int_eq(isCorrectInit(gameInfo), 1);
  ck_assert_int_eq(gameInfo->speed, 1);
  ck_assert_int_eq(*state, START);
  free(gameInfo->next);
  free(gameInfo->field);
}

START_TEST(test_fsm_3) {
  GameState_t *state = getState();
  GameInfo_t *gameInfo = getGameInfo();
  gameInfo->speed = 2;
  *state = START;
  UserAction_t action = Up;
  userInput(action, 1);
  ck_assert_int_eq(gameInfo->speed, 3);
  ck_assert_int_eq(*state, START);
  free(gameInfo->next);
  free(gameInfo->field);
}

START_TEST(test_fsm_3_a) {
  GameState_t *state = getState();
  GameInfo_t *gameInfo = getGameInfo();
  *state = START;
  UserAction_t action = Start;
  userInput(action, 1);
  ck_assert_int_eq(*state, SPAWN);
  free(gameInfo->next);
  free(gameInfo->field);
}

START_TEST(test_fsm_4) {
  GameState_t *state = getState();
  TetrominoInfo_t *tetromino = getTetromino();
  GameInfo_t *gameInfo = getGameInfo();
  *state = SPAWN;
  UserAction_t action = -1;
  userInput(action, 1);
  ck_assert_int_eq(*state, MOVING);
  ck_assert_int_eq(tetromino->x, 3);
  free(gameInfo->next);
  free(gameInfo->field);
  free(tetromino->map);
}

START_TEST(test_fsm_5) {
  GameState_t *state = getState();
  TetrominoInfo_t *tetromino = getTetromino();
  int **figureBefore = createMatrix(5, 5);
  copyFigureFromLeftToRight(tetromino->map, figureBefore);
  GameInfo_t *gameInfo = getGameInfo();
  *state = MOVING;
  UserAction_t action = Action;
  userInput(action, 1);
  int isEqual = isEqualFigure(figureBefore, tetromino->map);
  int isSquare = isFigureSquare();
  ck_assert_int_eq(((!isEqual) || isSquare), 1);
  free(gameInfo->next);
  free(gameInfo->field);
  free(tetromino->map);
  free(figureBefore);
}

START_TEST(test_fsm_6) {
  GameState_t *state = getState();
  TetrominoInfo_t *tetromino = getTetromino();
  GameInfo_t *gameInfo = getGameInfo();
  int y = tetromino->y;
  *state = MOVING;
  UserAction_t action = Down;
  userInput(action, 1);
  ck_assert_int_eq(y != tetromino->y, 1);
  free(gameInfo->next);
  free(gameInfo->field);
  free(tetromino->map);
}

START_TEST(test_fsm_7) {
  GameState_t *state = getState();
  TetrominoInfo_t *tetromino = getTetromino();
  GameInfo_t *gameInfo = getGameInfo();
  int x = tetromino->x;
  *state = MOVING;
  UserAction_t action = Left;
  userInput(action, 1);
  ck_assert_int_eq(x - 1, tetromino->x);
  free(gameInfo->next);
  free(gameInfo->field);
  free(tetromino->map);
}

START_TEST(test_fsm_8) {
  GameState_t *state = getState();
  TetrominoInfo_t *tetromino = getTetromino();
  GameInfo_t *gameInfo = getGameInfo();
  int x = tetromino->x;
  *state = MOVING;
  UserAction_t action = Right;
  userInput(action, 1);
  ck_assert_int_eq(x + 1, tetromino->x);
  free(gameInfo->next);
  free(gameInfo->field);
  free(tetromino->map);
}

START_TEST(test_fsm_9) {
  GameState_t *state = getState();
  TetrominoInfo_t *tetromino = getTetromino();
  GameInfo_t *gameInfo = getGameInfo();
  *state = MOVING;
  UserAction_t action = Pause;
  userInput(action, 1);
  ck_assert_int_eq(gameInfo->pause, 1);
  free(gameInfo->next);
  free(gameInfo->field);
  free(tetromino->map);
}

START_TEST(test_fsm_10) {
  GameState_t *state = getState();
  *state = MOVING;
  UserAction_t action = Terminate;
  userInput(action, 1);
  ck_assert_int_eq(*state, GAMEOVER);
}

START_TEST(test_fsm_11) {
  GameState_t *state = getState();
  TetrominoInfo_t *tetromino = getTetromino();
  GameInfo_t *gameInfo = getGameInfo();
  int y = tetromino->y;
  *state = SHIFTING;
  UserAction_t action = -1;
  userInput(action, 1);
  ck_assert_int_eq(tetromino->y, y + 1);
  free(gameInfo->next);
  free(gameInfo->field);
  free(tetromino->map);
}

START_TEST(test_fsm_12) {
  GameState_t *state = getState();
  TetrominoInfo_t *tetromino = getTetromino();
  GameInfo_t *gameInfo = getGameInfo();
  *state = COLLIDE;
  UserAction_t action = -1;
  userInput(action, 1);
  ck_assert_int_eq(*state, SPAWN);
  free(gameInfo->next);
  free(gameInfo->field);
  free(tetromino->map);
}

START_TEST(test_fsm_13) {
  GameState_t *state = getState();
  TetrominoInfo_t *tetromino = getTetromino();
  GameInfo_t *gameInfo = getGameInfo();
  *state = COLLIDE;
  gameInfo->field[5][0] = 1;
  UserAction_t action = -1;
  userInput(action, 1);
  ck_assert_int_eq(*state, GAMEOVER);
  free(gameInfo->next);
  free(gameInfo->field);
  free(tetromino->map);
}

START_TEST(test_fsm_14) {
  GameState_t *state = getState();
  TetrominoInfo_t *tetromino = getTetromino();
  GameInfo_t *gameInfo = getGameInfo();
  *state = COLLIDE;
  int y_pos = tetromino->y + findBottomBlockPos(tetromino->map);
  gameInfo->field[y_pos][0] = 1;
  gameInfo->field[y_pos][1] = 1;
  gameInfo->field[y_pos][2] = 1;
  gameInfo->field[y_pos][3] = 1;
  gameInfo->field[y_pos][4] = 1;
  gameInfo->field[y_pos][5] = 1;
  gameInfo->field[y_pos][6] = 1;
  gameInfo->field[y_pos][7] = 1;
  gameInfo->field[y_pos][8] = 1;
  gameInfo->field[y_pos][9] = 1;
  UserAction_t action = -1;
  userInput(action, 1);
  ck_assert_int_eq(gameInfo->score, 100);
  free(gameInfo->next);
  free(gameInfo->field);
  free(tetromino->map);
}
START_TEST(test_fsm_15) {
  GameState_t *state = getState();
  GameInfo_t *gameInfo = getGameInfo();
  *state = GAMEOVER;
  UserAction_t action = -1;
  userInput(action, 1);
  ck_assert_int_eq(gameInfo->pause, 2);
}

Suite *fsm_test(void) {
  Suite *suite;
  TCase *core;

  suite = suite_create("fsm_test");
  core = tcase_create("core");
  tcase_add_test(core, test_fsm_1);
  tcase_add_test(core, test_fsm_2);
  tcase_add_test(core, test_fsm_3);
  tcase_add_test(core, test_fsm_3_a);
  tcase_add_test(core, test_fsm_4);
  tcase_add_test(core, test_fsm_5);
  tcase_add_test(core, test_fsm_6);
  tcase_add_test(core, test_fsm_7);
  tcase_add_test(core, test_fsm_8);
  tcase_add_test(core, test_fsm_9);
  tcase_add_test(core, test_fsm_10);
  tcase_add_test(core, test_fsm_11);
  tcase_add_test(core, test_fsm_12);
  tcase_add_test(core, test_fsm_13);
  tcase_add_test(core, test_fsm_14);
  tcase_add_test(core, test_fsm_15);

  suite_add_tcase(suite, core);

  return (suite);
}

int isEqualFigure(int **matrix1, int **matrix2) {
  int isEqual = 1;
  for (int i = 0; i < 5 && isEqual; i++) {
    for (int j = 0; j < 5; j++) {
      if (matrix1[i][j] != matrix2[i][j]) {
        isEqual = 0;
        break;
      }
    }
  }
  return isEqual;
}