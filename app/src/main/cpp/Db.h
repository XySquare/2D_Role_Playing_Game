//
// Created by Xyy on 2018/12/26.
//

#ifndef RPG2D_DB_H
#define RPG2D_DB_H


#include <string>
#include "Monster.h"

class Db {

public:
    static Monster* getMonsterById(int monsterId){
        if(monsterId == 1){
            return new Monster(0,new Property(100,1,1,1),10,5);
        }
        return NULL;
    }
};


#endif //RPG2D_DB_H
