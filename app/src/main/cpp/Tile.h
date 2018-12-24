//
// Created by Xyy on 2018/12/22.
//

#ifndef RPG2D_TILE_H
#define RPG2D_TILE_H


class Tile {

protected:

    const int gid;

public:

    Tile(const int gid) : gid(gid) {}

    virtual int getGId(float stateTime){

        return gid;
    };
};


#endif //RPG2D_TILE_H
