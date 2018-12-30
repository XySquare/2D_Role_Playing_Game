//
// Created by Xyy on 2018/12/23.
//

#ifndef RPG2D_MAPLOADER_H
#define RPG2D_MAPLOADER_H


#include "rapidjson/document.h"
#include "JsonFiles.h"
#include "Map.h"
#include "TileLayer.h"
#include "ObjectLayer.h"

#define  LOG_TAG    "MapLoader"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

class MapLoader {

private:

    const char *read(const char *fileName) {
        using namespace rpg;
        if (strcmp(fileName, "tiled_map00.json") == 0)
            return tiled_map00;
        if (strcmp(fileName, "tiled_map01.json") == 0)
            return tiled_map01;
        if (strcmp(fileName, "GoP_1.json") == 0)
            return GoP_1;
        if (strcmp(fileName, "GoP_6.json") == 0)
            return GoP_6;
        if (strcmp(fileName, "obj.json") == 0)
            return obj;
        if (strcmp(fileName, "monster.json") == 0)
            return monster;
        LOGI("File Not Found: %s", fileName);
        return NULL;
    }

    int
    findTileSet(rapidjson::Value &data, int dataCount, int *tileSetsFirstGid, int tileSetsCount) {

        // Find a gid > 0
        int gid = 0;
        for (int j = 0; j < dataCount; j++) {
            gid = data[j].GetInt();
            if (gid > 0) {
                break;
            }
        }
        int i;
        for (i = 0; i < tileSetsCount && gid >= tileSetsFirstGid[i]; i++);
        return i - 1;
    }

    int findTileSet(int gid, int *tileSetsFirstGid, int tileSetsCount) {

        int i;
        for (i = 0; i < tileSetsCount && gid >= tileSetsFirstGid[i]; i++);
        return i - 1;
    }

public:

    Map *loadMap(const char *fileName) {
        using namespace rapidjson;
        LOGI("World Loading...%s.", fileName);

        const char *mapData = read(fileName);

        Document mapObj;
        mapObj.Parse(mapData);

        int width = mapObj["width"].GetInt();

        int height = mapObj["height"].GetInt();

        int tileHeight = mapObj["tileheight"].GetInt();

        int tileWidth = mapObj["tilewidth"].GetInt();

        /**
         * Parse TileSets
         */

        Value &tileSetsAry = mapObj["tilesets"];

        SizeType tileSetCount = tileSetsAry.Size();

        int *tileSetsFirstGid = new int[tileSetCount];

        TileSet **tileSets = new TileSet *[tileSetCount];

        for (SizeType i = 0; i < tileSetCount; i++) {

            int firstgid = tileSetsAry[i]["firstgid"].GetInt();
            tileSetsFirstGid[i] = firstgid;
            const char *source = tileSetsAry[i]["source"].GetString();
            tileSets[i] = loadTileSet(source);
        }

        /**
         * Parse Map Layers
         */

        int tileLayerCount = 0;

        int *collisionLayer = NULL;

        Value &layersAry = mapObj["layers"];

        SizeType layerCount = layersAry.Size();

        MapLayer **mapLayersTemp = new MapLayer *[layerCount];

        for (int i = 0; i < layerCount; i++) {

            Value &layerObj = layersAry[i];

            const char *type = layerObj["type"].GetString();

            LOGI("Layer...%s.", type);

            // TileLayer
            if (strcmp(type, "tilelayer") == 0) {

                Value &data = layerObj["data"];

                SizeType dataCount = data.Size();

                const char *name = layerObj["name"].GetString();

                if (strcmp(name, "collisionLayer") == 0) {

                    collisionLayer = new int[dataCount];
                    for (int j = 0; j < dataCount; j++) {

                        collisionLayer[j] = data[j].GetInt();
                    }
                } else {

                    //Determine which TileSet this layer will use
                    int tileSetInedx = findTileSet(data, dataCount, tileSetsFirstGid, tileSetCount);
                    if (tileSetInedx >= 0) {

                        TileSet *tileSet = tileSets[tileSetInedx];
                        int firstGid = tileSetsFirstGid[tileSetInedx];

                        Tile **tiles = new Tile *[dataCount];
                        for (int j = 0; j < dataCount; j++) {
                            int gid = data[j].GetInt();
                            if (gid > 0)
                                tiles[j] = tileSet->tiles[gid - firstGid];
                            else
                                tiles[j] = NULL;
                        }
                        mapLayersTemp[tileLayerCount] = new TileLayer(tiles, tileSetInedx);
                        tileLayerCount++;
                    }
                }
            }
                // ObjectLayer
            else if (strcmp(type, "objectgroup") == 0) {

                Value &objects = layerObj["objects"];

                SizeType objectCount = objects.Size();

                //Determine which TileSet this layer will use
                int tileSetInedx = -1;

                for (int j = 0; j < objectCount; j++) {

                    Value &object = objects[j];

                    if (object.HasMember("gid")) {
                        int gid = object["gid"].GetInt();

                        tileSetInedx = findTileSet(gid, tileSetsFirstGid, tileSetCount);
                    }
                }

                TileSet *tileSet = tileSets[tileSetInedx];

                int firstGid = tileSetsFirstGid[tileSetInedx];

                MapObject **mapObjectTemp = new MapObject *[objectCount];

                int mapObjectCount = 0;

                for (int j = 0; j < objectCount; j++) {

                    Value &object = objects[j];

                    // Tile Object
                    if (object.HasMember("gid")) {
                        int id = object["id"].GetInt();
                        const char *objType = object["type"].GetString();
                        const char *objName = object["name"].GetString();
                        int objWidth = object["width"].GetInt();
                        int objHeight = object["height"].GetInt();
                        int x = object["x"].GetInt();
                        // Fix position
                        int y = object["y"].GetInt() - objHeight;
                        int gid = object["gid"].GetInt();
                        Value::ConstMemberIterator itr = object.FindMember("properties");
                        if (itr != object.MemberEnd()) {
                            const Value &properties = itr->value;
                            SizeType propertyCount = properties.Size();
                            if (propertyCount > 0) {
                                const Value &prop = properties[0];
                                int intProperty = prop["value"].GetInt();
                                mapObjectTemp[mapObjectCount] = new MapObject(id, x, y, objType,
                                                                              objName, objWidth,
                                                                              objHeight,
                                                                              tileSet->tiles[gid -
                                                                                             firstGid],
                                                                              intProperty);
                                mapObjectCount++;
                            } else {
                                // W/Properties Length = 0
                                LOGI("Properties Length = 0");
                            }
                        } else {
                            mapObjectTemp[mapObjectCount] = new MapObject(id, x, y, objType,
                                                                          objName, objWidth,
                                                                          objHeight,
                                                                          tileSet->tiles[gid -
                                                                                         firstGid]);
                            mapObjectCount++;
                        }
                    }
                        // Polygon
                    else if (object.HasMember("polyline")) {

                    }
                        // Ellipse
                    else if (object.HasMember("ellipse")) {

                    }
                        // Rectangle
                    else {
                        int id = object["id"].GetInt();
                        int objWidth = object["width"].GetInt();
                        int objHeight = object["height"].GetInt();
                        int x = object["x"].GetInt();
                        int y = object["y"].GetInt();

                        mapObjectTemp[mapObjectCount] = new MapObject(id, x, y, objWidth,
                                                                      objHeight, NULL);
                        mapObjectCount++;
                    }
                }

                // Copy objects
                MapObject **mapObjects = new MapObject *[mapObjectCount];
                for (int j = 0; j < mapObjectCount; j++) {
                    mapObjects[j] = mapObjectTemp[j];
                }
                delete[] mapObjectTemp;

                LOGI("Object Count:%d", mapObjectCount);

                mapLayersTemp[tileLayerCount] = new ObjectLayer(mapObjectCount, mapObjects,
                                                                tileSetInedx);
                tileLayerCount++;
            }
        }

        // Copy layers
        MapLayer **mapLayers = new MapLayer *[tileLayerCount];
        for (int i = 0; i < tileLayerCount; i++) {
            mapLayers[i] = mapLayersTemp[i];
        }
        delete[] mapLayersTemp;

        delete[] tileSetsFirstGid;

        LOGI("Map Loaded.");

        return new Map(tileSetCount, tileSets, width, height, tileWidth, tileHeight, collisionLayer,
                       tileLayerCount, mapLayers);
    }

    TileSet *loadTileSet(const char *fileName) {
        using namespace rapidjson;

        LOGI("TileSet Loading...%s.", fileName);

        const char *tileSetData = read(fileName);

        Document tileSetObj;
        tileSetObj.Parse(tileSetData);

        const char *image = tileSetObj["image"].GetString();

        int tileCount = tileSetObj["tilecount"].GetInt();

        int columns = tileSetObj["columns"].GetInt();

        int tileHeight = tileSetObj["tileheight"].GetInt();

        int tileWidth = tileSetObj["tilewidth"].GetInt();

        int imageHeight = tileSetObj["imageheight"].GetInt();

        int imageWidth = tileSetObj["imagewidth"].GetInt();

        Tile **const tiles = new Tile *[tileCount];

        int tileId = 0;

        if (tileSetObj.HasMember("tiles")) {

            const Value &tilesAry = tileSetObj["tiles"];

            SizeType tilesAryCount = tilesAry.Size();

            for (int i = 0; i < tilesAryCount; i++) {

                const Value &tileObj = tilesAry[i];

                if (!tileObj.HasMember("animation"))
                    continue;

                int id = tileObj["id"].GetInt();

                while (tileId < id) {
                    tiles[tileId] = new Tile(tileId);
                    tileId++;
                }

                const Value &animation = tileObj["animation"];

                SizeType animationCount = animation.Size();

                float duration = animation[0]["duration"].GetInt() / 1000.f;

                int *frames = new int[animationCount];

                for (int f = 0; f < animationCount; f++) {
                    frames[f] = animation[f]["tileid"].GetInt();
                }

                tiles[tileId] = new AnimeTile(duration, animationCount, frames);

                tileId++;
            }
        }

        while (tileId < tileCount) {
            tiles[tileId] = new Tile(tileId);
            tileId++;
        }
        LOGI("TileSet Loaded.");

        return new TileSet(image, tileCount, columns, tileHeight, tileWidth, imageHeight,
                           imageWidth, tiles);
    }
};


#endif //RPG2D_MAPLOADER_H
