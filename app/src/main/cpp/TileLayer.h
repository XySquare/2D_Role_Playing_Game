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

    TileLayer(const unsigned char id, Tile **tiles, unsigned char tileSetIndex) :
            MapLayer(id, TILE_LAYER, tileSetIndex), tiles(tiles){}

    virtual ~TileLayer() override {

        delete[] tiles;
    }
};


#endif //RPG2D_TILELAYER_H
