//
// Created by Xyy on 2018/12/22.
//

#ifndef RPG2D_TILE_H
#define RPG2D_TILE_H


class Tile {

protected:

    // This field serve as the Gid of Tile, but serve as the frameLength of AnimeTile
    const int gid;

public:

    Tile(const int gid) : gid(gid) {}

    virtual int getGId(float stateTime){

        return gid;
    };

    virtual ~Tile() {

    }
};


#endif //RPG2D_TILE_H
