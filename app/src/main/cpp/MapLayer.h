//
// Created by Xyy on 2018/12/23.
//

#ifndef RPG2D_MAPLAYER_H
#define RPG2D_MAPLAYER_H


class MapLayer {

public:

    static const int TILE_LAYER = 0;

    static const int OBJECT_LAYER = 1;

    const char layerType;

    MapLayer(const char layerType) : layerType(layerType) {}

    int getLayerType(){

        return layerType;
    };
};


#endif //RPG2D_MAPLAYER_H
