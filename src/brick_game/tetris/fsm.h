#ifndef FSM_H
#define FSM_H

#include "backend.h"
#include "tetris.h"

// void userInput(UserAction_t action, bool hold);
void onStartState(UserAction_t signal);
void onSpawnState();
void onMovingState(UserAction_t signal);
void onShiftingState();
void onCollideState();
void onGameOverState();

#endif