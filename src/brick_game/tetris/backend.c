#include "backend.h"

#include <stdbool.h>

GameInfo_t *getGameInfo() {
  static GameInfo_t info;
  static int initialised = false;
  if (!initialised) {
    initGame(&info);
    initialised = true;
  }
  return &info;
}

GameState_t *getState() {
  static GameState_t state = START;
  static int initialised = false;
  if (!initialised) {
    state = START;
    initialised = true;
  }
  return &state;
}

TimeInfo_t *getTime() {
  static TimeInfo_t time;
  static int initialised = false;
  if (!initialised) {
    clock_gettime(CLOCK_MONOTONIC, &time.start);
    initialised = true;
  }
  return &time;
}

TetrominoInfo_t *getTetromino() {
  static TetrominoInfo_t tetromino;
  static int initialised = false;
  if (!initialised) {
    tetromino.x = 3;
    tetromino.map = createMatrix(5, 5);
    if (tetromino.map != NULL) {
      GameInfo_t *gameInfo = getGameInfo();
      if (gameInfo->field != NULL)
        copyFigureFromLeftToRight(gameInfo->next, tetromino.map);
    }
    tetromino.y = findBottomBlockPos(tetromino.map);
    initialised = true;
  }
  return &tetromino;
}

GameInfo_t updateCurrentState() {
  GameInfo_t *gameInfo = getGameInfo();
  return *gameInfo;
}

void initGame(GameInfo_t *gameInfo) {
  int status = ERROR;
  gameInfo->field = createMatrix(ROWS_FIELD + TETROMINO_SIDE, COLUMNS_FIELD);
  if (gameInfo->field) {
    status = SUCCESS;
    gameInfo->next = createMatrix(5, 5);
    srand(time(NULL));
    int **newTetromino = getRandomFigure();
    copyFigureFromLeftToRight(newTetromino, gameInfo->next);
    free(newTetromino);
  }
  if (status == SUCCESS) {
    gameInfo->score = 0;
    gameInfo->pause = 0;
    gameInfo->speed = 1;
    gameInfo->level = 1;
    gameInfo->high_score = getHiScore();
  } else
    gameInfo = NULL;
}

bool isCorrectInit() {
  int isCorrectUnit = false;
  GameInfo_t *gameInfo = getGameInfo();
  TetrominoInfo_t *tetromino = getTetromino();
  if (gameInfo->field != NULL && tetromino->map != NULL) isCorrectUnit = true;
  return isCorrectUnit;
}

int **createMatrix(int rows, int columns) {
  int **matrix = allocBuffer(rows, columns);
  if (matrix) {
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < columns; j++) {
        matrix[i][j] = 0;
      }
    }
  }
  return matrix;
}

int **allocBuffer(int rows, int column) {
  int **array = malloc((column * rows * sizeof(int)) + (rows * sizeof(int *)));
  if (array) {
    int *pointer = (int *)(array + rows);
    for (int i = 0; i < rows; i++) {
      array[i] = pointer + column * i;
    }
  }
  return array;
}

bool isCollide() {
  GameInfo_t *gameInfo = getGameInfo();
  TetrominoInfo_t *tetromino = getTetromino();
  bool isCollide = false;
  eraseTetromino(tetromino);
  int bottomBlockPos = findBottomBlockPos(tetromino->map);
  if (gameInfo->field != NULL && tetromino->map != NULL) {
    for (int i = tetromino->y, i_fig = 0; i_fig < TETROMINO_SIDE && !isCollide;
         i++, i_fig++) {
      for (int j = tetromino->x, j_fig = 0; j_fig < TETROMINO_SIDE;
           j++, j_fig++) {
        if ((i + bottomBlockPos) == (ROWS_FIELD + TETROMINO_SIDE + 2)) {
          isCollide = true;
          break;
        } else if (tetromino->map[i_fig][j_fig] == 1) {
          if (gameInfo->field[i + 1][j] != 0) {
            isCollide = true;
            break;
          }
        }
      }
    }
  }
  addTetrominoToField(tetromino);
  return isCollide;
}

int randomTetromino() {
  int figure = 0;
  figure = rand() % 7;
  return figure;
}

int **getRandomFigure() {
  int figure_0[5][5] = {{0, 0, 1, 0, 0},
                        {0, 0, 1, 0, 0},
                        {0, 0, 1, 0, 0},
                        {0, 0, 1, 0, 0},
                        {0, 0, 0, 0, 0}};
  int figure_1[5][5] = {{0, 0, 0, 0, 0},
                        {0, 1, 0, 0, 0},
                        {0, 1, 1, 1, 0},
                        {0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0}};
  int figure_2[5][5] = {{0, 0, 0, 0, 0},
                        {0, 0, 0, 1, 0},
                        {0, 1, 1, 1, 0},
                        {0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0}};
  int figure_3[5][5] = {{0, 0, 0, 0, 0},
                        {0, 0, 1, 1, 0},
                        {0, 0, 1, 1, 0},
                        {0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0}};
  int figure_4[5][5] = {{0, 0, 0, 0, 0},
                        {0, 0, 1, 1, 0},
                        {0, 1, 1, 0, 0},
                        {0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0}};
  int figure_5[5][5] = {{0, 0, 0, 0, 0},
                        {0, 0, 1, 0, 0},
                        {0, 1, 1, 1, 0},
                        {0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0}};
  int figure_6[5][5] = {{0, 0, 0, 0, 0},
                        {0, 1, 1, 0, 0},
                        {0, 0, 1, 1, 0},
                        {0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0}};
  int(*figures[7])[5] = {figure_0, figure_1, figure_2, figure_3,
                         figure_4, figure_5, figure_6};

  int **matrix = createMatrix(5, 5);
  int figureIndex = randomTetromino();
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      matrix[i][j] = figures[figureIndex][i][j];
    }
  }
  return matrix;
}

void copyFigureFromLeftToRight(int **source, int **destination) {
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      destination[i][j] = source[i][j];
    }
  }
}

void addTetrominoToField(TetrominoInfo_t *tetromino) {
  GameInfo_t *gameInfo = getGameInfo();
  if (gameInfo->field != NULL && tetromino->map != NULL) {
    for (int i = tetromino->y, i_fig = 0; i_fig < TETROMINO_SIDE;
         i++, i_fig++) {
      for (int j = tetromino->x, j_fig = 0; j_fig < TETROMINO_SIDE;
           j++, j_fig++) {
        if (tetromino->map[i_fig][j_fig] == 1) {
          gameInfo->field[i][j] = 1;
        }
      }
    }
  }
}

void eraseTetromino(TetrominoInfo_t *tetromino) {
  GameInfo_t *gameInfo = getGameInfo();
  if (tetromino->map != NULL) {
    for (int i = tetromino->y, i_fig = 0; i_fig < TETROMINO_SIDE;
         i++, i_fig++) {
      for (int j = tetromino->x, j_fig = 0; j_fig < TETROMINO_SIDE;
           j++, j_fig++) {
        if (tetromino->map[i_fig][j_fig] == 1) {
          gameInfo->field[i][j] = 0;
        }
      }
    }
  }
}

void shiftRowsDownFrom(int y) {
  GameInfo_t *gameInfo = getGameInfo();
  for (int i = y; i >= 4; i--) {
    for (int j = 0; j < COLUMNS_FIELD; j++) {
      gameInfo->field[i][j] = gameInfo->field[i - 1][j];
    }
  }
}

bool isFillRow(int y) {
  GameInfo_t *gameInfo = getGameInfo();
  bool isFilled = true;
  for (int j = 0; j < COLUMNS_FIELD && isFilled; j++) {
    if (gameInfo->field[y][j] == 0) isFilled = false;
  }
  return isFilled;
}

void handleFilledRows() {
  TetrominoInfo_t *tetromino = getTetromino();
  GameInfo_t *gameInfo = getGameInfo();
  int countFillRow = 0;
  int bottomBlockPos = findBottomBlockPos(tetromino->map);
  for (int i = tetromino->y; i <= tetromino->y + bottomBlockPos; i++)
    if (isFillRow(i)) {
      countFillRow += 1;
      shiftRowsDownFrom(i);
    }
  if (countFillRow > 0) {
    int bit = 1;
    bit = bit << countFillRow;
    gameInfo->score += (bit - 1) * 100;
    if (gameInfo->score > gameInfo->high_score) {
      gameInfo->high_score = gameInfo->score;
      saveHighScore(gameInfo->high_score);
    }
  }
}

void handleLevel() {
  GameInfo_t *gameInfo = getGameInfo();
  int level = (gameInfo->score / 600) + 1;
  if (level <= 10 && level > gameInfo->level) {
    gameInfo->level = level;
    gameInfo->speed = level;
  }
}

void moveLeft() {
  TetrominoInfo_t *tetromino = getTetromino();
  eraseTetromino(tetromino);
  if (!isForbiddenMoveLeft(tetromino)) {
    tetromino->x -= 1;
  }
  addTetrominoToField(tetromino);
}

void moveRight() {
  TetrominoInfo_t *tetromino = getTetromino();
  eraseTetromino(tetromino);
  if (!isForbiddenMoveRight(tetromino)) {
    tetromino->x += 1;
  }
  addTetrominoToField(tetromino);
}

void moveDown() {
  TetrominoInfo_t *tetromino = getTetromino();
  eraseTetromino(tetromino);
  tetromino->y += 1;
  addTetrominoToField(tetromino);
}

int findLeftBlockPos(int **map) {
  int left_position = -1;
  for (int j = 0; j < 3; j++) {
    for (int i = 0; i < 5; i++) {
      if (map[i][j] == 1) {
        left_position = j;
        break;
      }
    }
    if (left_position >= 0) break;
  }
  return left_position;
}

int findRightBlockPos(int **map) {
  int rightPosition = 0;
  for (int j = 4; j >= 2; j--) {
    for (int i = 4; i >= 0; i--) {
      if (map[i][j] == 1) {
        rightPosition = j;
        break;
      }
    }
    if (rightPosition != 0) break;
  }
  return rightPosition;
}

int findBottomBlockPos(int **map) {
  int bottom_position = 0;
  for (int i = 4; i >= 2; i--) {
    for (int j = 0; j < 5; j++) {
      if (map[i][j] == 1) {
        bottom_position = i;
        break;
      }
    }
    if (bottom_position != 0) break;
  }
  return bottom_position;
}

int findXCoordAfterRotate(int **map) {
  TetrominoInfo_t *tetromino = getTetromino();
  int xAfterRotate = tetromino->x;
  int rightBlockPos = findRightBlockPos(map);
  int leftBlockPos = findLeftBlockPos(map);
  if ((tetromino->x + rightBlockPos) >= COLUMNS_FIELD)
    xAfterRotate = COLUMNS_FIELD - rightBlockPos - 1;
  else if ((leftBlockPos + tetromino->x) < 0)
    xAfterRotate = 0 - leftBlockPos;
  return xAfterRotate;
}

bool isForbiddenMoveLeft(TetrominoInfo_t *tetromino) {
  GameInfo_t *gameInfo = getGameInfo();
  int isForbiddenMoveLeft = false;
  if ((findLeftBlockPos(tetromino->map) + tetromino->x) <= 0)
    isForbiddenMoveLeft = true;
  else {
    for (int i = tetromino->y, i_fig = 0; i_fig < 5 && !isForbiddenMoveLeft;
         i++, i_fig++) {
      for (int j = tetromino->x, j_fig = 0; j_fig < 3; j++, j_fig++) {
        if (tetromino->map[i_fig][j_fig] == 1 &&
            gameInfo->field[i][j - 1] == 1) {
          isForbiddenMoveLeft = true;
          break;
        }
      }
    }
  }
  return isForbiddenMoveLeft;
}

bool isForbiddenMoveRight(TetrominoInfo_t *tetromino) {
  GameInfo_t *gameInfo = getGameInfo();
  bool isForbiddenMoveRight = false;
  if ((tetromino->x + findRightBlockPos(tetromino->map) + 1) >= COLUMNS_FIELD)
    isForbiddenMoveRight = true;
  else {
    for (int i = tetromino->y, i_fig = 0; i_fig < 5 && !isForbiddenMoveRight;
         i++, i_fig++) {
      for (int j = tetromino->x + 4, j_fig = 4; j_fig > 1; j--, j_fig--) {
        if (tetromino->map[i_fig][j_fig] == 1 &&
            gameInfo->field[i][j + 1] == 1) {
          isForbiddenMoveRight = true;
          break;
        }
      }
    }
  }
  return isForbiddenMoveRight;
}

bool isAvailableToAdd(int tempX, int **rotated_temp) {
  GameInfo_t *gameInfo = getGameInfo();
  TetrominoInfo_t *tetromino = getTetromino();
  bool isAvailableToAdd = true;
  if (gameInfo->field != NULL) {
    for (int i = tetromino->y, i_fig = 0;
         i_fig < TETROMINO_SIDE && isAvailableToAdd; i++, i_fig++) {
      for (int j = tempX, j_fig = 0; j_fig < TETROMINO_SIDE; j++, j_fig++) {
        if (rotated_temp[i_fig][j_fig] == 1) {
          if (gameInfo->field[i][j] != 0) {
            isAvailableToAdd = false;
            break;
          }
        }
      }
    }
  }
  return isAvailableToAdd;
}

void rotate() {
  TetrominoInfo_t *tetromino = getTetromino();
  eraseTetromino(tetromino);
  int **temp_matrix = createMatrix(5, 5);
  int **rotated_temp = createMatrix(5, 5);
  copyFigureFromLeftToRight(tetromino->map, temp_matrix);
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) rotated_temp[j][i] = temp_matrix[i][5 - j - 1];
  }
  int tempX = findXCoordAfterRotate(rotated_temp);
  if (isAvailableToAdd(tempX, rotated_temp)) {
    tetromino->x = tempX;
    copyFigureFromLeftToRight(rotated_temp, tetromino->map);
  }
  free(temp_matrix);
  free(rotated_temp);
  addTetrominoToField(tetromino);
}

bool isFigureSquare() {
  TetrominoInfo_t *tetromino = getTetromino();
  return (tetromino->map[1][2] && tetromino->map[1][3] &&
          tetromino->map[2][2] && tetromino->map[2][3])
             ? true
             : false;
}

bool isFieldFull() {
  GameInfo_t *gameInfo = getGameInfo();
  bool isFieldFull = false;
  for (int i = 0; i < COLUMNS_FIELD; i++) {
    if (gameInfo->field[5][i] == 1) {
      isFieldFull = true;
      break;
    }
  }
  return isFieldFull;
}

int getHiScore() {
  int hi_score = 0;
  FILE *fp = fopen("hi_score.txt", "r");
  if (fp) {
    fscanf(fp, "%d", &hi_score);
    fclose(fp);
  }
  return hi_score;
}

void saveHighScore(int highScore) {
  FILE *fp = fopen("hi_score.txt", "w");
  if (fp) {
    fprintf(fp, "%d", highScore);
    fclose(fp);
  }
}

void changeSpeed(int speed) {
  GameInfo_t *gameInfo = getGameInfo();
  if (speed == -1) {
    if (gameInfo->speed > 1) {
      gameInfo->speed -= 1;
      gameInfo->level -= 1;
    }
  } else if (speed == 1) {
    if (gameInfo->speed < 10) {
      gameInfo->speed += 1;
      gameInfo->level += 1;
    }
  }
}