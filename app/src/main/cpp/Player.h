//
// Created by Xyy on 2018/12/19.
//

#ifndef RPG2D_PLAYER_H
#define RPG2D_PLAYER_H


#include "DynamicGameObject.h"


//const float Player::PLAYER_VELOCITY = 360.f;

class Player: public DynamicGameObject {
private:

    static const int PLAYER_VELOCITY = 400;

public:

    static const int PLAYER_STATE_DOWN = 1;

    static const int PLAYER_STATE_LEFT = 2;

    static const int PLAYER_STATE_UP = 3;

    static const int PLAYER_STATE_RIGHT = 4;

    static const int PLAYER_WIDTH = 62;

    static const int PLAYER_HEIGHT = 32;

    int state = PLAYER_STATE_DOWN;

    float stateTime = 0;

    Player(float x, float y) : DynamicGameObject(x, y) {}

    void update(float deltaTime) {

        position.x += PLAYER_VELOCITY * velocity.x * deltaTime;
        position.y += PLAYER_VELOCITY * velocity.y * deltaTime;

        if (velocity.x != 0 || velocity.y != 0) {
            stateTime += deltaTime;
            if (velocity.y > 0.7071)
                state = PLAYER_STATE_DOWN;
            else if (velocity.y < -0.7071)
                state = PLAYER_STATE_UP;
            else if (velocity.x > 0)
                state = PLAYER_STATE_RIGHT;
            else
                state = PLAYER_STATE_LEFT;
        }else{
            stateTime = 0;
        }
    }
};


#endif //RPG2D_PLAYER_H
