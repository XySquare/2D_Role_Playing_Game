//
// Created by Xyy on 2018/12/20.
//

#ifndef RPG2D_MAP_H
#define RPG2D_MAP_H


#include <vector>
#include "MapObject.h"
#include "Tile.h"
#include "MapLayer.h"
#include "TileSet.h"

class Map {

public:

    const int tileSetsCount;

    TileSet **const tileSets;

    const int width;

    const int height;

    const int tileWidth;

    const int tileHeight;

    const int *collisionLayer;

    const int layerCount;

    MapLayer **const layers;

    std::vector<MapObject> objects;

    Map(int tileSetsCount, TileSet **const tileSets, const int width, const int height, const int tileWidth,
        const int tileHeight, const int *collisionLayer, const int layerCount, MapLayer **const layers) : tileSetsCount(tileSetsCount),tileSets(
            tileSets), width(width), height(height), tileWidth(tileWidth), tileHeight(tileHeight),
                                                                                    collisionLayer(
                                                                                            collisionLayer),
                                                                                    layerCount(layerCount),
                                                                                    layers(layers) {
        //Test
        objects.push_back(MapObject(0,1408.00,384.00,64,64,0));
    }


};


#endif //RPG2D_MAP_H
