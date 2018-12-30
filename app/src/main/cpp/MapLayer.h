//
// Created by Xyy on 2018/12/23.
//

#ifndef RPG2D_MAPLAYER_H
#define RPG2D_MAPLAYER_H


class MapLayer {

public:

    enum {
        TILE_LAYER = 0,
        OBJECT_LAYER
    };

    const unsigned char layerType;

    const unsigned int tileSetIndex;

    MapLayer(const unsigned char layerType, const unsigned int tileSetIndex) :
            layerType(layerType), tileSetIndex(tileSetIndex) {}

    virtual ~MapLayer(){

    }
};


#endif //RPG2D_MAPLAYER_H
