//
// Created by Xyy on 2018/12/21.
//

#ifndef RPG2D_MAPOBJECT_H
#define RPG2D_MAPOBJECT_H


//#include <string>
#include "GameObject.h"
#include "Tile.h"

class MapObject : public GameObject {

public:

    int id;

    std::string type;

    std::string name;

    int width;

    int height;

    Tile *tile;

    int properties;

    //float rotation;

    //bool visible;

    MapObject(int id, float x, float y, int width, int height, Tile *tile) :
            GameObject(x, y), id(id), width(width), height(height), tile(tile) {}

    MapObject(int id, float x, float y, const std::string &type, const std::string &name, int width,
              int height, Tile *tile) : GameObject(x, y), id(id), type(type), name(name),
                                        width(width), height(height), tile(tile), properties(0) {}

    MapObject(int id, float x, float y, const std::string &type, const std::string &name, int width,
              int height, Tile *tile, int properties) : GameObject(x, y), id(id), type(type),
                                                        name(name), width(width), height(height),
                                                        tile(tile), properties(properties) {}

    virtual ~MapObject() {
        // tile will be delete in TileSet
    }
};


#endif //RPG2D_MAPOBJECT_H
