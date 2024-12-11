#ifndef TETRIS_H
#define TETRIS_H

#include <stdbool.h>

#include "../../defines.h"
#include "../../structs.h"

GameInfo_t updateCurrentState();
void userInput(UserAction_t action, bool hold);
bool isCorrectInit();

#endif