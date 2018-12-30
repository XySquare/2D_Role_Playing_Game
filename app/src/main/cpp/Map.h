//
// Created by Xyy on 2018/12/20.
//

#ifndef RPG2D_MAP_H
#define RPG2D_MAP_H


#include "MapLayer.h"
#include "TileSet.h"

class Map final{

public:

    const int tileSetCount;

    TileSet **const tileSets;

    const int width;

    const int height;

    const int tileWidth;

    const int tileHeight;

    const int *collisionLayer;

    const int layerCount;

    MapLayer **const layers;

    Map(int tileSetCount, TileSet **const tileSets,
        const int width, const int height,
        const int tileWidth, const int tileHeight,
        const int *collisionLayer,
        const int layerCount, MapLayer **const layers) :
            tileSetCount(tileSetCount), tileSets(tileSets),
            width(width), height(height),
            tileWidth(tileWidth), tileHeight(tileHeight),
            collisionLayer(collisionLayer),
            layerCount(layerCount), layers(layers) {}

    ~Map() {

        for (int i = 0; i < tileSetCount; ++i)
            delete tileSets[i];
        delete[] tileSets;

        for (int i = 0; i < layerCount; ++i)
            delete layers[i];
        delete[] layers;

        delete[] collisionLayer;
    }
};


#endif //RPG2D_MAP_H
