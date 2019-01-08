//
// Created by Xyy on 2018/12/23.
//

#ifndef RPG2D_MAPLAYER_H
#define RPG2D_MAPLAYER_H


class MapLayer {

public:

    enum Type: unsigned char{
        TILE_LAYER = 0,
        OBJECT_LAYER
    };

    const unsigned char id;

    const Type layerType;

    const unsigned char tileSetIndex;

    MapLayer(const unsigned char id, const Type layerType, const unsigned char tileSetIndex) :
            id(id), layerType(layerType), tileSetIndex(tileSetIndex) {}

    virtual ~MapLayer(){

    }
};


#endif //RPG2D_MAPLAYER_H
