#ifndef FRONTEND_H
#define FRONTEND_H

#define _XOPEN_SOURCE 500
#include <ncurses.h>
#include <unistd.h>

#include "../../defines.h"
#include "../../structs.h"

void printOverlay(void);
void printRectangle(int top_y, int bottom_y, int left_x, int right_x);
void printBrick(int y, int x);
void clearBrick(int y, int x);
void printField(GameInfo_t gameInfo);
void printNextFigure(GameInfo_t gameInfo);
void printInfo(GameInfo_t gameInfo);
void printEndBanner(GameInfo_t gameInfo);
void printSnake();
UserAction_t getAction(int user_input);

#endif