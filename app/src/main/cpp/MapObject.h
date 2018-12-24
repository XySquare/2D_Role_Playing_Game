//
// Created by Xyy on 2018/12/21.
//

#ifndef RPG2D_MAPOBJECT_H
#define RPG2D_MAPOBJECT_H


//#include <string>
#include "GameObject.h"

class MapObject: public GameObject {

public:

    int id;

    int width;

    int height;

    int gid;

    //std::string name;

    //float rotation;

    //std::string type;

    //bool visible;

    MapObject(int id, float x, float y, int width, int height, int gid) : GameObject(x, y), id(id),
                                                                          width(width),
                                                                          height(height),
                                                                          gid(gid) {}
};


#endif //RPG2D_MAPOBJECT_H
