//
// Created by Xyy on 2018/12/19.
//

#ifndef RPG2D_GAMEOBJECT_H
#define RPG2D_GAMEOBJECT_H

#include "Vector2.h"

class GameObject {

public:

    Vector2 position;

    //Rectangle bounds;

    GameObject(float x, float y) : position(Vector2(x,y)) {}

    virtual ~GameObject() {

    }
};


#endif //RPG2D_GAMEOBJECT_H
