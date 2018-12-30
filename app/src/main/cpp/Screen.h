//
// Created by Xyy on 2018/12/16.
//

#ifndef RPG2D_SCREEN_H
#define RPG2D_SCREEN_H


#include "MultiTouchHandler.h"

class Screen {

public:

    virtual void resume() = 0;

    virtual void update(float deltaTime, MultiTouchHandler *handler) = 0;

    virtual void present() = 0;

    virtual ~Screen() {

    }
};


#endif //RPG2D_SCREEN_H
