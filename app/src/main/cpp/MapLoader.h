//
// Created by Xyy on 2018/12/23.
//

#ifndef RPG2D_MAPLOADER_H
#define RPG2D_MAPLOADER_H


#include "rapidjson/document.h"
#include "JsonFiles.h"
#include "TileSet.h"
#include "MapLayer.h"
#include "TileLayer.h"
#include "Map.h"

class MapLoader {

private:

    const char* read(const char* fileName){
        return tiled_map00;
    }

    int findTileSet(rapidjson::Value& data, int dataCount, int *tileSetsFirstGid, int tileSetsCount){

        // Find a gid > 0
        int gid = 0;
        for (int j = 0; j < dataCount; j++) {
            gid = data[j].GetInt();
            if(gid > 0){
                break;
            }
        }
        int i;
        for(i = 0;i < tileSetsCount && gid >= tileSetsFirstGid[i];i++);
        return i - 1;
    }

public:

    Map* loadMap(const char* fileName){
        using namespace rapidjson;

        const char* mapData = read("dummy");

        Document mapObj;
        mapObj.Parse(mapData);

        Value& tileSetsAry = mapObj["tilesets"];
        SizeType tileSetsCount = tileSetsAry.Size();
        int *tileSetsFirstGid = new int[tileSetsCount];
        TileSet **tileSets = new TileSet*[tileSetsCount];
        for (SizeType i = 0; i < tileSetsCount; i++){

            int firstgid = tileSetsAry[i]["firstgid"].GetInt();
            tileSetsFirstGid[i] = firstgid;
            const char* source = tileSetsAry[i]["source"].GetString();
            tileSets[i] = loadTileSet(source);
        }

        int width = mapObj["width"].GetInt();

        int height = mapObj["height"].GetInt();

        int tileHeight = mapObj["tileheight"].GetInt();

        int tileWidth = mapObj["tilewidth"].GetInt();

        /*
         * Parse Layers
         * */

        int tileLayerCount = 0;

        int *collisionLayer = NULL;

        Value& layersAry = mapObj["layers"];

        SizeType layerCount = layersAry.Size();

        MapLayer **mapLayersTemp = new MapLayer*[layerCount];

        for(int i=0;i<layerCount;i++) {

            Value& layerObj = layersAry[i];

            const char* type = layerObj["type"].GetString();

            if(strcmp(type,"tilelayer") == 0){

                Value& data = layerObj["data"];

                SizeType dataCount = data.Size();

                const char * name = layerObj["name"].GetString();

                if(strcmp(name,"collisionLayer") == 0){

                    collisionLayer = new int[dataCount];
                    for (int j = 0; j < dataCount; j++) {

                        collisionLayer[j] = data[j].GetInt();
                    }
                }else{

                    //Determine which TileSet this layer will use
                    int tileSetInedx = findTileSet(data, dataCount, tileSetsFirstGid, tileSetsCount);
                    if(tileSetInedx >= 0){

                        TileSet *tileSet = tileSets[tileSetInedx];

                        Tile **tiles = new Tile *[dataCount];
                        for (int j = 0; j < dataCount; j++) {
                            int gid = data[j].GetInt();
                            if(gid > 0)
                                tiles[j] = tileSet->tiles[gid - 1];
                            else
                                tiles[j] = NULL;
                        }
                        mapLayersTemp[i] = new TileLayer(tiles, tileSetInedx);

                        tileLayerCount++;
                    }
                }
            }

        }

        // Copy layers
        MapLayer **mapLayers = new MapLayer*[tileLayerCount];
        for(int i=0;i<tileLayerCount;i++){
            mapLayers[i] = mapLayersTemp[i];
        }
        delete mapLayersTemp;


        return new Map(tileSetsCount,tileSets,width,height,tileWidth,tileHeight,collisionLayer,tileLayerCount,mapLayers);
    }

    TileSet* loadTileSet(const char* fileName){
        using namespace rapidjson;

        //read(fileName);
        const char* tileSetData = GoP_1;

        Document tileSetObj;
        tileSetObj.Parse(tileSetData);

        const char* image = tileSetObj["image"].GetString();

        int tileCount = tileSetObj["tilecount"].GetInt();

        int columns = tileSetObj["columns"].GetInt();

        int tileHeight = tileSetObj["tileheight"].GetInt();

        int tileWidth = tileSetObj["tilewidth"].GetInt();

        int imageHeight = tileSetObj["imageheight"].GetInt();

        int imageWidth = tileSetObj["imagewidth"].GetInt();

        Tile** const tiles = new Tile*[tileCount];

        int tileId = 0;

        const Value& tilesAry = tileSetObj["tiles"];

        SizeType tilesAryCount = tilesAry.Size();

        for(int i=0;i<tilesAryCount;i++){

            const Value& tileObj = tilesAry[i];

            int id = tileObj["id"].GetInt();

            while (tileId < id){
                tiles[tileId] = new Tile(tileId);
                tileId++;
            }

            const Value& animation = tileObj["animation"];

            SizeType animationCount = animation.Size();

            float duration = animation[0]["duration"].GetInt()/1000.f;

            int *frames = new int[animationCount];

            for(int f=0;f<animationCount;f++){
                frames[f] = animation[f]["tileid"].GetInt();
            }

            tiles[tileId] = new AnimeTile(duration,animationCount,frames);

            tileId++;
        }

        while (tileId < tileCount){
            tiles[tileId] = new Tile(tileId);
            tileId++;
        }

        return new TileSet(image,tileCount,columns,tileHeight,tileWidth,imageHeight,imageWidth,tiles);
    }
};


#endif //RPG2D_MAPLOADER_H
