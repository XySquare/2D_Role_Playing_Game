//
// Created by Xyy on 2018/12/26.
//

#ifndef RPG2D_PROPERTY_H
#define RPG2D_PROPERTY_H


class Property {

public:

    unsigned short lv;

    unsigned short maxHp;

    // During battle, HP might be negative
    signed short hp;

    unsigned short atk;

    unsigned short def;

    unsigned short agi;

    Property(unsigned short lv, unsigned short maxHp, unsigned short atk, unsigned short def,
             unsigned short agi) : lv(lv), maxHp(maxHp), hp(maxHp), atk(atk),
                                   def(def), agi(agi) {}

    Property(const Property &other) {
        lv = other.lv;
        maxHp = other.maxHp;
        hp = other.hp;
        atk = other.atk;
        def = other.def;
        agi = other.agi;
    }
};


#endif //RPG2D_PROPERTY_H
