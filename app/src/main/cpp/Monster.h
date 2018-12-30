//
// Created by Xyy on 2018/12/29.
//

#ifndef RPG2D_MONSTER_H
#define RPG2D_MONSTER_H


#include "Property.h"

class Monster {

public:

    int gid;

    Property *prop;

    int coin;

    int exp;

    Monster(int gid, Property *prop, int coin, int exp) : gid(gid), prop(prop), coin(coin),
                                                          exp(exp) {}

    virtual ~Monster() {

        delete prop;
    }

};


#endif //RPG2D_MONSTER_H
