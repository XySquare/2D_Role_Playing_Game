//
// Created by Xyy on 2018/12/22.
//

#ifndef RPG2D_TILESET_H
#define RPG2D_TILESET_H


#include "Tile.h"
#include "AnimeTile.h"

class TileSet final{

public:

    char *image;

    const int tileCount;

    const int columns;

    const int tileHeight;

    const int tileWidth;

    const int imageHeight;

    const int imageWidth;

    Tile **const tiles;

    TileSet(const char* pImage, const int tileCount, const int columns,
            const int tileHeight, const int tileWidth,
            const int imageHeight, const int imageWidth,
            Tile **const tiles) :
            tileCount(tileCount), columns(columns),
            tileHeight(tileHeight), tileWidth(tileWidth),
            imageHeight(imageHeight), imageWidth(imageWidth),
            tiles(tiles) {

        image = new char[strlen(pImage)];
        strcpy(image, pImage);
    }

    ~TileSet() {

        delete[] image;

        for(int i = 0; i < tileCount; ++i)
            delete tiles[i];
        delete[] tiles;
    }
};


#endif //RPG2D_TILESET_H
