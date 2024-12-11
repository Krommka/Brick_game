#include "brick_game.h"

int main(void) {
  initscr();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  printOverlay();
  gameLoop();
  endwin();
  return SUCCESS;
}

void gameLoop() {
  UserAction_t action = 0;
  bool break_flag = isCorrectInit();

  while (break_flag) {
    action = GET_USER_INPUT;
    userInput(getAction(action), 1);
    GameInfo_t gameInfo = updateCurrentState();
    printInfo(gameInfo);
    if (gameInfo.pause == 0) {
      printField(gameInfo);
      printNextFigure(gameInfo);
    } else if (gameInfo.pause == 2) {
      break_flag = FALSE;
      printEndBanner(gameInfo);
    }
  }
}
