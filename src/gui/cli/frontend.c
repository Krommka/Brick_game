#include "frontend.h"

void printOverlay(void) {
  printRectangle(0, BOARD_N, 0, BOARD_M + 1);
  printRectangle(0, BOARD_N, BOARD_M + 2, BOARD_M + HUD_WIDTH + 4);

  printRectangle(1, 3, BOARD_M + 3, BOARD_M + HUD_WIDTH + 3);
  printRectangle(4, 6, BOARD_M + 3, BOARD_M + HUD_WIDTH + 3);
  printRectangle(7, 13, BOARD_M + 3, BOARD_M + HUD_WIDTH + 3);
  printRectangle(14, 16, BOARD_M + 3, BOARD_M + HUD_WIDTH + 3);
  printRectangle(17, 19, BOARD_M + 3, BOARD_M + HUD_WIDTH + 3);

  MVPRINTW(1, BOARD_M + 8, "SCORE");
  MVPRINTW(4, BOARD_M + 7, "HI SCORE");
  MVPRINTW(7, BOARD_M + 8, "NEXT");
  MVPRINTW(14, BOARD_M + 8, "LEVEL");
  MVPRINTW(17, BOARD_M + 8, "SPEED");

  MVPRINTW(BOARD_N / 2, (BOARD_M - INTRO_MESSAGE_LEN) / 2 + 1, INTRO_MESSAGE);
}

void printRectangle(int top_y, int bottom_y, int left_x, int right_x) {
  MVADDCH(top_y, left_x, ACS_ULCORNER);

  int i = left_x + 1;

  for (; i < right_x; i++) MVADDCH(top_y, i, ACS_HLINE);
  MVADDCH(top_y, i, ACS_URCORNER);

  for (int i = top_y + 1; i < bottom_y; i++) {
    MVADDCH(i, left_x, ACS_VLINE);
    MVADDCH(i, right_x, ACS_VLINE);
  }

  MVADDCH(bottom_y, left_x, ACS_LLCORNER);
  i = left_x + 1;
  for (; i < right_x; i++) MVADDCH(bottom_y, i, ACS_HLINE);
  MVADDCH(bottom_y, i, ACS_LRCORNER);
}

void printBrick(int y, int x) {
  start_color();
  init_pair(1, COLOR_BLACK, COLOR_WHITE);
  attron(COLOR_PAIR(1));
  if (y > 4) MVPRINTW((y - 4), (x * 2 + 1), "  ");
  attroff(COLOR_PAIR(1));
}

void clearBrick(int y, int x) {
  if (y > 4) MVPRINTW((y - 4), (x * 2 + 1), "  ");
}

void printField(GameInfo_t gameInfo) {
  for (int i = 5; i < ROWS_FIELD + TETROMINO_SIDE - 1; i++) {
    for (int j = 0; j < COLUMNS_FIELD; j++) {
      if (gameInfo.field[i][j] == 1)
        printBrick(i, j);
      else
        clearBrick(i, j);
    }
  }
}

void printNextFigure(GameInfo_t gameInfo) {
  if (gameInfo.next != NULL) {
    for (int i = 12, i_fig = 0; i_fig < TETROMINO_SIDE; i++, i_fig++) {
      for (int j = 12, j_fig = 0; j_fig < TETROMINO_SIDE - 1; j++, j_fig++) {
        if (gameInfo.next[i_fig][j_fig] == 1)
          printBrick(i, j);
        else
          clearBrick(i, j);
      }
    }
  }
}

void printInfo(GameInfo_t gameInfo) {
  MVPRINTW(2, BOARD_M + 5, "%8d", gameInfo.score);
  MVPRINTW(5, BOARD_M + 5, "%8d", gameInfo.high_score);
  MVPRINTW(15, BOARD_M + 5, "%8d", gameInfo.level);
  MVPRINTW(18, BOARD_M + 5, "%8d", gameInfo.speed);
}

UserAction_t getAction(int user_input) {
  UserAction_t rc = NO_INPUT;

  if (user_input == KEY_UP)
    rc = Up;
  else if (user_input == KEY_DOWN)
    rc = Down;
  else if (user_input == KEY_LEFT)
    rc = Left;
  else if (user_input == KEY_RIGHT)
    rc = Right;
  else if (user_input == SPACE)
    rc = Action;
  else if (user_input == ESCAPE)
    rc = Terminate;
  else if (user_input == PAUSE)
    rc = Pause;
  else if (user_input == ENTER_KEY)
    rc = Start;

  return rc;
}
void printEndBanner(GameInfo_t gameInfo) {
  printSnake();
  start_color();
  init_pair(2, COLOR_BLACK, COLOR_WHITE);
  attron(COLOR_PAIR(2));
  MVPRINTW(BOARD_N / 2 - 1, (BOARD_M - 9) / 2, "GAME OVER");
  MVPRINTW(BOARD_N / 2, (BOARD_M - 10) / 2, "You earned");
  MVPRINTW(BOARD_N / 2 + 1, (BOARD_M - 10) / 2, "%d points", gameInfo.score);
  attroff(COLOR_PAIR(2));
  wrefresh(stdscr);
  usleep(2000000);
}

void printSnake() {
  int flag = 0;
  int c = 0;
  int i = 0;
  int j = 0;
  int left_corner = 0;
  int up_corner = 1;
  int right_corner = COLUMNS_FIELD;
  int bottom_corner = ROWS_FIELD - 1;
  while (c <= (COLUMNS_FIELD * ROWS_FIELD)) {
    if (flag == 0) {
      while (j < right_corner) {
        printBrick(i + 5, j);
        wrefresh(stdscr);
        usleep(10000);
        j++;
        c++;
      }
      j--;
      i++;
      right_corner--;
      flag = 1;
    } else if (flag == 1) {
      while (i < bottom_corner) {
        printBrick(i + 5, j);
        wrefresh(stdscr);
        usleep(10000);
        i++;
        c++;
      }
      i--;
      j--;
      bottom_corner--;
      flag = 2;
    } else if (flag == 2) {
      while (j >= left_corner) {
        printBrick(i + 5, j);
        wrefresh(stdscr);
        usleep(10000);
        j--;
        c++;
      }
      j++;
      i--;
      left_corner++;
      flag = 3;
    } else if (flag == 3) {
      while (i >= up_corner) {
        printBrick(i + 5, j);
        wrefresh(stdscr);
        usleep(10000);
        i--;
        c++;
      }
      i++;
      j++;
      up_corner++;
      flag = 0;
    }
  }
}