//
// Created by Xyy on 2018/12/31.
//

#ifndef RPG2D_GAME_H
#define RPG2D_GAME_H


#include "FileIO.h"
#include "MultiTouchHandler.h"

struct Game {

    FileIO *fileIO;

    MultiTouchHandler *input;

    Game(FileIO *fileIO, MultiTouchHandler *multiTouchHandler) :
            fileIO(fileIO), input(multiTouchHandler) {}
};


#endif //RPG2D_GAME_H
