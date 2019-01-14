//
// Created by Xyy on 2018/12/23.
//

#ifndef RPG2D_MAPLOADER_H
#define RPG2D_MAPLOADER_H


#include "rapidjson/document.h"
#include "Map.h"
#include "TileLayer.h"
#include "ObjectLayer.h"
#include "Log.h"


class MapLoader {

private:

    /**
     * The string returned should be delete []
     */
    const char *read(FileIO *fileIO, const char *fileName) {

        AAsset *file = fileIO->readAssetFile(fileName);
        size_t fileLength = static_cast<size_t>(AAsset_getLength(file));
        // Allocate memory to read file
        char *fileContent = new char[fileLength + 1];
        AAsset_read(file, fileContent, fileLength);
        AAsset_close(file);
        fileContent[fileLength] = '\0';
        return fileContent;
    }

    unsigned char
    findTileSet(rapidjson::Value &data, unsigned int dataCount, unsigned int *tileSetsFirstGid,
                unsigned char tileSetsCount) {

        // Find a gid > 0
        unsigned int gid = 0;
        for (unsigned int j = 0; j < dataCount; j++) {
            gid = static_cast<unsigned int>(data[j].GetInt());
            if (gid > 0) {
                break;
            }
        }
        unsigned char i;
        for (i = 0; i < tileSetsCount && gid >= tileSetsFirstGid[i]; i++);
        assert(i > 0);
        return static_cast<unsigned char>(i - 1);
    }

    unsigned char
    findTileSet(unsigned int gid, unsigned int *tileSetsFirstGid, unsigned char tileSetsCount) {

        unsigned char i;
        for (i = 0; i < tileSetsCount && gid >= tileSetsFirstGid[i]; i++);
        return static_cast<unsigned char>(i - 1);
    }

    unsigned char hex2dec(const char *hex) {

        unsigned char res = 0;
        unsigned char k = 1;
        for (signed char i = 1; i >= 0; i--) {

            char c = hex[i];
            unsigned char n = 0;
            if (c >= '0' && c <= '9') {
                n = static_cast<unsigned char>(c - '0');
            } else if (c >= 'A' && c <= 'F') {
                n = static_cast<unsigned char>(c - 'A' + 10);
            } else if (c >= 'a' && c <= 'f') {
                n = static_cast<unsigned char>(c - 'a' + 10);
            }
            res += n * k;
            k *= 16;
        }
        return res;
    }

public:

    Map *loadMap(FileIO *fileIO, const char *fileName) {
        using namespace rapidjson;
        LOGI("MapLoader","World Loading...%s.", fileName);

        const char *mapData = read(fileIO, fileName);

        Document mapObj;
        mapObj.Parse(mapData);

        delete[] mapData;

        if (strcmp(mapObj["type"].GetString(), "map") != 0) {
            LOGE("MapLoader","You are not loading a map.");
        }

        unsigned int width = static_cast<unsigned int>(mapObj["width"].GetInt());

        unsigned int height = static_cast<unsigned int>(mapObj["height"].GetInt());

        unsigned int tileHeight = static_cast<unsigned int>(mapObj["tileheight"].GetInt());

        unsigned int tileWidth = static_cast<unsigned int>(mapObj["tilewidth"].GetInt());

        // (Optional, default: black) Background Color
        Vector3 backgroundColor(0,0,0);
        Value::ConstMemberIterator backgroundColorItr = mapObj.FindMember("backgroundcolor");
        if (backgroundColorItr != mapObj.MemberEnd()) {
            const char *backgroundColorHex = backgroundColorItr->value.GetString();
            if (strlen(backgroundColorHex) == 7) {
                float red = hex2dec(backgroundColorHex + 1)/255.f;
                float green = hex2dec(backgroundColorHex + 3)/255.f;
                float blue = hex2dec(backgroundColorHex + 5)/255.f;
                backgroundColor = Vector3(red, green, blue);
            }
        }

        Vector2 defaultLocation(-1,-1);

        /**
         * Parse TileSets
         */

        Value &tileSetAry = mapObj["tilesets"];

        unsigned char tileSetCount = static_cast<unsigned char>(tileSetAry.Size());

        unsigned int *tileSetFirstGids = new unsigned int[tileSetCount];

        TileSet **tileSets = new TileSet *[tileSetCount];

        for (unsigned char i = 0; i < tileSetCount; i++) {

            tileSetFirstGids[i] = static_cast<unsigned int>(tileSetAry[i]["firstgid"].GetInt());
            tileSets[i] = loadTileSet(fileIO, tileSetAry[i]["source"].GetString());
        }

        /**
         * Parse Map Layers
         */

        unsigned char layerCount = 0;

        unsigned char *collisionLayer = NULL;

        Value &layerAry = mapObj["layers"];

        SizeType layerArySize = layerAry.Size();

        MapLayer **mapLayersTemp = new MapLayer *[layerArySize];

        for (unsigned char i = 0; i < layerArySize; i++) {

            Value &layerObj = layerAry[i];

            const unsigned char layerId = static_cast<const unsigned char>(layerObj["id"].GetInt());

            const char *type = layerObj["type"].GetString();

            // TileLayer
            if (strcmp(type, "tilelayer") == 0) {

                Value &dataAry = layerObj["data"];

                SizeType dataCount = dataAry.Size();

                const char *layerName = layerObj["name"].GetString();

                if (strcmp(layerName, "collisionLayer") == 0) {

                    collisionLayer = new unsigned char[dataCount];
                    for (SizeType j = 0; j < dataCount; j++) {

                        collisionLayer[j] = static_cast<unsigned char>(dataAry[j].GetInt());
                    }
                } else {

                    //Determine which TileSet this layer will use
                    unsigned char tileSetInedx = findTileSet(dataAry, dataCount, tileSetFirstGids,
                                                             tileSetCount);

                    TileSet *tileSet = tileSets[tileSetInedx];
                    unsigned int firstGid = tileSetFirstGids[tileSetInedx];

                    Tile **tiles = new Tile *[dataCount];
                    for (unsigned int j = 0; j < dataCount; j++) {
                        unsigned int gid = static_cast<unsigned int>(dataAry[j].GetInt());
                        if (gid > firstGid + tileSet->tileCount)
                            LOGE("MapLoader","Invalid Gid: %d", gid);
                        else if (gid > 0)
                            tiles[j] = tileSet->tiles[gid - firstGid];
                        else
                            tiles[j] = NULL;
                    }

                    mapLayersTemp[layerCount++] = new TileLayer(layerId, tiles, tileSetInedx);
                }
            }
            // ObjectLayer
            else if (strcmp(type, "objectgroup") == 0) {

                Value &objectAry = layerObj["objects"];

                SizeType objectAryCount = objectAry.Size();

                //Determine which TileSet this layer will use
                // Note: It's possible that no TileSet will be used in ObjectLayer
                // When all objects are invisible
                unsigned char tileSetInedx = 0;

                for (unsigned int j = 0; j < objectAryCount; j++) {

                    Value &objectObj = objectAry[j];

                    if (objectObj.HasMember("gid")) {

                        unsigned int gid = static_cast<unsigned int>(objectObj["gid"].GetInt());
                        tileSetInedx = findTileSet(gid, tileSetFirstGids, tileSetCount);
                    }
                }

                TileSet *tileSet = tileSets[tileSetInedx];

                unsigned int firstGid = tileSetFirstGids[tileSetInedx];

                MapObject **mapObjectsTemp = new MapObject *[objectAryCount];

                unsigned int objectCount = 0;

                for (unsigned int j = 0; j < objectAryCount; j++) {

                    Value &objectObj = objectAry[j];

                    // Polygon (Not supported)
                    if (objectObj.HasMember("polyline")) {
                        continue;
                    }
                    // Ellipse (Not supported)
                    else if (objectObj.HasMember("ellipse")) {
                        continue;
                    }

                    const char *objName = objectObj["name"].GetString();
                    int x = objectObj["x"].GetInt();
                    int y = objectObj["y"].GetInt();

                    // Point
                    if (objectObj.HasMember("point")) {
                        if(strcmp(objName,"defaultLocation") == 0){
                            defaultLocation = Vector2(x,y);
                        }
                        continue;
                    }

                    unsigned int objId = static_cast<unsigned int>(objectObj["id"].GetInt());
                    const char *objType = objectObj["type"].GetString();
                    unsigned int objWidth = static_cast<unsigned int>(objectObj["width"].GetInt());
                    unsigned int objHeight = static_cast<unsigned int>(objectObj["height"].GetInt());

                    Tile *tile = NULL;
                    Value::ConstMemberIterator itr = objectObj.FindMember("properties");
                    unsigned char propertyCount = 0;
                    ObjectProperty **objectProperties = NULL;
                    if (itr != objectObj.MemberEnd()) {
                        const Value &properties = itr->value;
                        propertyCount = static_cast<unsigned char>(properties.Size());
                        if (propertyCount > 0) {
                            objectProperties = new ObjectProperty *[propertyCount];
                            for (unsigned char k = 0; k < propertyCount; k++) {
                                const Value &prop = properties[k];
                                const char *propertyName = prop["name"].GetString();
                                const char *propertyType = prop["type"].GetString();
                                if (strcmp(propertyType, "int") == 0) {
                                    objectProperties[k] = new IntProperty(propertyName,
                                                                          prop["value"].GetInt());
                                } else if (strcmp(propertyType, "string") == 0) {
                                    objectProperties[k] = new StringProperty(propertyName,
                                                                             prop["value"].GetString());
                                } else {
                                    LOGI("MapLoader","Unsupported DataType.");
                                }
                            }
                        }
                    }

                    // Tile Object
                    if (objectObj.HasMember("gid")) {

                        // Fix position
                        y -= objHeight;

                        unsigned int gid = static_cast<unsigned int>(objectObj["gid"].GetInt());
                        tile = tileSet->tiles[gid - firstGid];
                    }

                    mapObjectsTemp[objectCount++] = new MapObject(objId, x, y,
                                                                  objType, objName,
                                                                  objWidth, objHeight,
                                                                  tile,
                                                                  propertyCount,
                                                                  objectProperties);
                }

                // Copy objects
                MapObject **mapObjects = new MapObject *[objectCount];
                for (int j = 0; j < objectCount; j++) {
                    mapObjects[j] = mapObjectsTemp[j];
                }
                delete[] mapObjectsTemp;

                mapLayersTemp[layerCount++] = new ObjectLayer(layerId, objectCount, mapObjects,
                                                              tileSetInedx);
            }
        }

        // Copy layers
        MapLayer **mapLayers = new MapLayer *[layerCount];
        for (int i = 0; i < layerCount; i++) {
            mapLayers[i] = mapLayersTemp[i];
        }
        delete[] mapLayersTemp;

        delete[] tileSetFirstGids;

        LOGI("MapLoader","Map Loaded.");

        return new Map(tileSetCount, tileSets, width, height, tileWidth, tileHeight, collisionLayer,
                       layerCount, mapLayers, backgroundColor, defaultLocation);
    }

    TileSet *loadTileSet(FileIO *fileIO, const char *fileName) {
        using namespace rapidjson;

        LOGI("MapLoader","TileSet Loading...%s.", fileName);

        const char *tileSetData = read(fileIO, fileName);

        Document tileSetObj;
        tileSetObj.Parse(tileSetData);

        delete[] tileSetData;

        if (strcmp(tileSetObj["type"].GetString(), "tileset") != 0) {
            LOGE("MapLoader","You are not loading a tileset.");
        }

        const char *image = tileSetObj["image"].GetString();

        unsigned int tileCount = static_cast<unsigned int>(tileSetObj["tilecount"].GetInt());

        unsigned int columns = static_cast<unsigned int>(tileSetObj["columns"].GetInt());

        unsigned int tileHeight = static_cast<unsigned int>(tileSetObj["tileheight"].GetInt());

        unsigned int tileWidth = static_cast<unsigned int>(tileSetObj["tilewidth"].GetInt());

        unsigned int imageHeight = static_cast<unsigned int>(tileSetObj["imageheight"].GetInt());

        unsigned int imageWidth = static_cast<unsigned int>(tileSetObj["imagewidth"].GetInt());

        Tile **const tiles = new Tile *[tileCount];

        unsigned int tileId = 0;

        if (tileSetObj.HasMember("tiles")) {

            const Value &tilesAry = tileSetObj["tiles"];

            SizeType tilesAryCount = tilesAry.Size();

            for (int i = 0; i < tilesAryCount; i++) {

                const Value &tileObj = tilesAry[i];

                if (!tileObj.HasMember("animation"))
                    continue;

                unsigned int id = static_cast<unsigned int>(tileObj["id"].GetInt());

                while (tileId < id) {
                    tiles[tileId] = new Tile(tileId);
                    tileId++;
                }

                const Value &animation = tileObj["animation"];

                SizeType animationCount = animation.Size();

                // Although Tiled supports vary ramerate, here just pick the first one
                float duration = animation[0]["duration"].GetInt() / 1000.f;

                unsigned int *frames = new unsigned int[animationCount];

                for (unsigned int f = 0; f < animationCount; f++) {
                    frames[f] = static_cast<unsigned int>(animation[f]["tileid"].GetInt());
                }

                tiles[tileId++] = new AnimeTile(duration, animationCount, frames);
            }
        }

        while (tileId < tileCount) {
            tiles[tileId] = new Tile(tileId);
            tileId++;
        }
        LOGI("MapLoader","TileSet Loaded.");

        return new TileSet(image, tileCount, columns, tileHeight, tileWidth, imageHeight,
                           imageWidth, tiles);
    }
};


#endif //RPG2D_MAPLOADER_H
