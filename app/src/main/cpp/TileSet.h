//
// Created by Xyy on 2018/12/22.
//

#ifndef RPG2D_TILESET_H
#define RPG2D_TILESET_H


#include "Tile.h"
#include "AnimeTile.h"

class TileSet final{

public:

    std::string image;

    const unsigned int tileCount;

    const unsigned int columns;

    const unsigned int tileHeight;

    const unsigned int tileWidth;

    const unsigned short imageHeight;

    const unsigned short imageWidth;

    Tile **const tiles;

    TileSet(const char* image, const unsigned int tileCount, const unsigned int columns,
            const unsigned int tileHeight, const unsigned int tileWidth,
            const unsigned short imageHeight, const unsigned short imageWidth,
            Tile **const tiles) :
            image(image),
            tileCount(tileCount), columns(columns),
            tileHeight(tileHeight), tileWidth(tileWidth),
            imageHeight(imageHeight), imageWidth(imageWidth),
            tiles(tiles) {}

    ~TileSet() {

        for(unsigned int i = 0; i < tileCount; ++i)
            delete tiles[i];
        delete[] tiles;
    }
};


#endif //RPG2D_TILESET_H
