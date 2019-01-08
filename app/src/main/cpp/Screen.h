//
// Created by Xyy on 2018/12/16.
//

#ifndef RPG2D_SCREEN_H
#define RPG2D_SCREEN_H


#include "EventListener.h"
#include "Game.h"

class Screen: public EventListener {

protected:

    const Game game;

public:

    Screen(Game game) : game(game) {}

    virtual void resume() = 0;

    virtual void update(float deltaTime) = 0;

    virtual void present() = 0;

    virtual ~Screen() {

    }
};


#endif //RPG2D_SCREEN_H
