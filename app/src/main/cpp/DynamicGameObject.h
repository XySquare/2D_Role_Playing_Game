//
// Created by Xyy on 2018/12/19.
//

#ifndef RPG2D_DYNAMICGAMEOBJECT_H
#define RPG2D_DYNAMICGAMEOBJECT_H


#include "GameObject.h"

class DynamicGameObject: public GameObject {

public:

    Vector velocity;

    DynamicGameObject(float x, float y) : GameObject(x, y), velocity(Vector(0,0)) {}

    virtual ~DynamicGameObject() override {

    }
};


#endif //RPG2D_DYNAMICGAMEOBJECT_H
