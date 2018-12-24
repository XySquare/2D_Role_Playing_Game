//
// Created by Xyy on 2018/12/23.
//

#ifndef RPG2D_TILELAYER_H
#define RPG2D_TILELAYER_H


#include "MapLayer.h"
#include "Tile.h"

class TileLayer: public MapLayer {

public:

    const int tileSetIndex;

    Tile **tiles;

    TileLayer(Tile **tiles, int tileSetIndex) : MapLayer(TILE_LAYER), tiles(tiles), tileSetIndex(tileSetIndex) {}

};


#endif //RPG2D_TILELAYER_H
