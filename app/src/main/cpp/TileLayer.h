//
// Created by Xyy on 2018/12/23.
//

#ifndef RPG2D_TILELAYER_H
#define RPG2D_TILELAYER_H


#include "MapLayer.h"
#include "Tile.h"

class TileLayer final: public MapLayer {

public:

    Tile **const tiles;

    TileLayer(Tile **tiles, unsigned int tileSetIndex) :
            MapLayer(TILE_LAYER, tileSetIndex), tiles(tiles){}

    virtual ~TileLayer() override {

        delete[] tiles;
    }
};


#endif //RPG2D_TILELAYER_H
