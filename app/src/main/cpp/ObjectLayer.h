//
// Created by Xyy on 2018/12/24.
//

#ifndef RPG2D_OBJECTLAYER_H
#define RPG2D_OBJECTLAYER_H


#include "MapObject.h"
#include "MapLayer.h"

class ObjectLayer final: public MapLayer {

public:

    unsigned int objectCount;

    MapObject ** objects;

    ObjectLayer(const unsigned char id, unsigned int objectCount, MapObject **objects, unsigned char tileSetIndex) :
            MapLayer(id, OBJECT_LAYER, tileSetIndex), objectCount(objectCount), objects(objects) {};

    void remove(MapObject *object) {

        for (unsigned int j = 0; j < objectCount; j++) {
            MapObject *obj = objects[j];
            if (obj == object) {
                delete object;
                for (unsigned int k = j; k < objectCount - 1; k++) {
                    objects[k] = objects[k + 1];
                }
                objectCount--;
                break;
            }
        }
    }

    virtual ~ObjectLayer() override {

        for(unsigned int i = 0; i < objectCount; ++i)
            delete objects[i];
        delete[] objects;
    }
};


#endif //RPG2D_OBJECTLAYER_H
