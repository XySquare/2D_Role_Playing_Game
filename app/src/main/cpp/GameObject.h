//
// Created by Xyy on 2018/12/19.
//

#ifndef RPG2D_GAMEOBJECT_H
#define RPG2D_GAMEOBJECT_H

#include "Vector.h"

class GameObject {

public:

    Vector position;

    //Rectangle bounds;

    GameObject(float x, float y) : position(Vector(x,y)) {}
};


#endif //RPG2D_GAMEOBJECT_H
