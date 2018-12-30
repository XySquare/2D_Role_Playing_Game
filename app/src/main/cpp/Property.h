//
// Created by Xyy on 2018/12/26.
//

#ifndef RPG2D_PROPERTY_H
#define RPG2D_PROPERTY_H


class Property {

public:

    int maxHp;

    int hp;

    int atk;

    int def;

    int agi;

    Property(int maxHp, int atk, int def, int agi) : maxHp(maxHp), hp(maxHp), atk(atk),
                                                     def(def), agi(agi) {}

    Property(const Property &other) {
        maxHp = other.maxHp;
        hp = other.hp;
        atk = other.atk;
        def = other.def;
        agi = other.agi;
    }
};


#endif //RPG2D_PROPERTY_H
