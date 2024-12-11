#ifndef DEFINES_H
#define DEFINES_H

#define WIN_INIT(time)    \
  {                       \
    initscr();            \
    noecho();             \
    curs_set(0);          \
    keypad(stdscr, TRUE); \
    timeout(time);        \
  }

#define GET_USER_INPUT getch()

#define MVADDCH(y, x, c) mvaddch(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), c)
#define MVPRINTW(y, x, ...) \
  mvprintw(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), __VA_ARGS__)

#define BOARDS_BEGIN 2
#define BOARD_N (ROWS_FIELD)
#define BOARD_M (COLUMNS_FIELD * 2)
#define HUD_WIDTH 14
#define ROWS_FIELD 20
#define COLUMNS_FIELD 10
#define BOARD_PADDING 1

#define TETROMINO_SIDE 5
#define SUCCESS 0
#define ERROR 1

#define NO_INPUT -1

#define INTRO_MESSAGE "Press ENTER to start"
#define INTRO_MESSAGE_LEN 20

#define INITIAL_TIMEOUT 30

#define ESCAPE 27
#define ENTER_KEY 10
#define PAUSE 112
#define SPACE 32

#endif