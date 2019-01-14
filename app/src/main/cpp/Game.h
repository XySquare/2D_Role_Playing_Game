//
// Created by Xyy on 2018/12/31.
//

#ifndef RPG2D_GAME_H
#define RPG2D_GAME_H


#include "SpriteBatcher.h"
#include "FileIO.h"
#include "MultiTouchHandler.h"

struct Game {

    SpriteBatcher *graphic;

    FileIO *fileIO;

    MultiTouchHandler *input;

    Game(SpriteBatcher *spriteBatcher, FileIO *fileIO, MultiTouchHandler *multiTouchHandler) :
            graphic(spriteBatcher), fileIO(fileIO), input(multiTouchHandler) {}
};


#endif //RPG2D_GAME_H
