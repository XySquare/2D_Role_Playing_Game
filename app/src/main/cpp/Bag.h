//
// Created by Xyy on 2018/12/30.
//

#ifndef RPG2D_BAG_H
#define RPG2D_BAG_H


class Bag {

public:

    unsigned short key[3] = {0, 0, 0};

    unsigned char potion_s = 0;

    unsigned char potion_l = 0;

    void reset(){

        for(unsigned char i=0;i<3;i++)
            key[i] = 0;
        potion_s = 0;
        potion_l = 0;
    }
};


#endif //RPG2D_BAG_H
