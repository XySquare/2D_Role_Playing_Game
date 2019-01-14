//
// Created by Xyy on 2018/12/20.
//

#ifndef RPG2D_MAP_H
#define RPG2D_MAP_H


#include "MapLayer.h"
#include "TileSet.h"
#include "Vector2.h"

class Map final{

public:

    const unsigned char tileSetCount;

    TileSet **const tileSets;

    const unsigned int width;

    const unsigned int height;

    const unsigned int tileWidth;

    const unsigned int tileHeight;

    const unsigned char *collisionLayer;

    const unsigned char layerCount;

    MapLayer **const layers;

    const Vector3 backgroundColor;

    const Vector2 defaultLocation;

    Map(unsigned char tileSetCount, TileSet **const tileSets, const unsigned int width,
            const unsigned int height, const unsigned int tileWidth, const unsigned int tileHeight,
            const unsigned char *collisionLayer, const unsigned char layerCount,
            MapLayer **const layers, const Vector3 backgroundColor, const Vector2 defaultLocation) :
            tileSetCount(tileSetCount), tileSets(tileSets),
            width(width), height(height),
            tileWidth(tileWidth), tileHeight(tileHeight),
            collisionLayer(collisionLayer),
            layerCount(layerCount), layers(layers), backgroundColor(backgroundColor),
            defaultLocation(defaultLocation) {}

    ~Map() {

        for (unsigned char i = 0; i < tileSetCount; ++i)
            delete tileSets[i];
        delete[] tileSets;

        for (unsigned char i = 0; i < layerCount; ++i)
            delete layers[i];
        delete[] layers;

        delete[] collisionLayer;
    }
};


#endif //RPG2D_MAP_H
