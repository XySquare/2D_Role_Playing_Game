//
// Created by Xyy on 2018/12/26.
//

#ifndef RPG2D_DB_H
#define RPG2D_DB_H


#include "Monster.h"
#include "Log.h"

class Db {

private:

    Monster **monsters = NULL;

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
    findTileSet(unsigned int gid, unsigned int *tileSetsFirstGid, unsigned char tileSetsCount) {

        unsigned char i;
        for (i = 0; i < tileSetsCount && gid >= tileSetsFirstGid[i]; i++);
        return static_cast<unsigned char>(i - 1);
    }

    void load(FileIO *fileIO, const char *fileName) {
        using namespace rapidjson;

        const char *mapData = read(fileIO, fileName);

        Document mapObj;
        mapObj.Parse(mapData);

        delete[] mapData;

        /**
         * Parse TileSets
         */

        Value &tileSetAry = mapObj["tilesets"];

        unsigned char tileSetCount = static_cast<unsigned char>(tileSetAry.Size());

        unsigned int *tileSetFirstGids = new unsigned int[tileSetCount];

        for (unsigned char i = 0; i < tileSetCount; i++) {

            tileSetFirstGids[i] = static_cast<unsigned int>(tileSetAry[i]["firstgid"].GetInt());
        }

        /**
         * Parse Map Layers
         */

        //unsigned char layerCount = 0;

        Value &layerAry = mapObj["layers"];

        SizeType layerArySize = layerAry.Size();

        for (unsigned char i = 0; i < layerArySize; i++) {

            Value &layerObj = layerAry[i];

            const char *type = layerObj["type"].GetString();

            // ObjectLayer
             if (strcmp(type, "objectgroup") == 0) {

                 const char *layerName = layerObj["name"].GetString();

                 if (strcmp(layerName, "Monster") == 0) {

                     Value &objectAry = layerObj["objects"];

                     SizeType objectAryCount = objectAry.Size();

                     //Determine which TileSet this layer will use
                     unsigned char tileSetInedx = 0;

                     for (unsigned int j = 0; j < objectAryCount; j++) {

                         Value &objectObj = objectAry[j];

                         if (objectObj.HasMember("gid")) {

                             unsigned int gid = static_cast<unsigned int>(objectObj["gid"].GetInt());
                             tileSetInedx = findTileSet(gid, tileSetFirstGids, tileSetCount);
                         }
                     }

                     unsigned int firstGid = tileSetFirstGids[tileSetInedx];

                     monsters = new Monster *[objectAryCount];

                     //unsigned int objectCount = 0;

                     for (unsigned int j = 0; j < objectAryCount; j++) {

                         Value &objectObj = objectAry[j];

                         // (Skip) Polygon
                         if (objectObj.HasMember("polyline")) {
                             continue;
                         }
                             // (Skip) Ellipse
                         else if (objectObj.HasMember("ellipse")) {
                             continue;
                         }

                         // (Skip if not) Tile Object
                         if (!objectObj.HasMember("gid")) {
                             continue;
                         }

                         //unsigned int objId = static_cast<unsigned int>(objectObj["id"].GetInt());
                         //const char *objType = objectObj["type"].GetString();
                         //const char *objName = objectObj["name"].GetString();
                         //unsigned int objWidth = static_cast<unsigned int>(objectObj["width"].GetInt());
                         //unsigned int objHeight = static_cast<unsigned int>(objectObj["height"].GetInt());
                         //int x = objectObj["x"].GetInt();
                         //int y = objectObj["y"].GetInt();
                         //Tile *tile = NULL;

                         // Property
                         int lv = 1;
                         int hp = 0;
                         int atk = 0;
                         int def = 0;
                         int agi = 0;
                         // Others
                         int monsterId = 0;
                         int coin = 0;
                         int exp = 0;
                         Value::ConstMemberIterator itr = objectObj.FindMember("properties");
                         unsigned char propertyCount = 0;
                         //ObjectProperty **objectProperties = NULL;
                         if (itr != objectObj.MemberEnd()) {
                             const Value &properties = itr->value;
                             propertyCount = static_cast<unsigned char>(properties.Size());
                             if (propertyCount > 0) {
                                 //objectProperties = new ObjectProperty *[propertyCount];
                                 for (unsigned char k = 0; k < propertyCount; k++) {
                                     const Value &prop = properties[k];
                                     const char *propertyName = prop["name"].GetString();
                                     const char *propertyType = prop["type"].GetString();
                                     if (strcmp(propertyType, "int") == 0) {
                                         if (strcmp(propertyName, "agi") == 0) {
                                             agi = prop["value"].GetInt();
                                         } else if (strcmp(propertyName, "atk") == 0) {
                                             atk = prop["value"].GetInt();
                                         } else if (strcmp(propertyName, "coin") == 0) {
                                             coin = prop["value"].GetInt();
                                         } else if (strcmp(propertyName, "def") == 0) {
                                             def = prop["value"].GetInt();
                                         } else if (strcmp(propertyName, "exp") == 0) {
                                             exp = prop["value"].GetInt();
                                         } else if (strcmp(propertyName, "hp") == 0) {
                                             hp = prop["value"].GetInt();
                                         } else if (strcmp(propertyName, "lv") == 0) {
                                             lv = prop["value"].GetInt();
                                         } else if (strcmp(propertyName, "monsterId") == 0) {
                                             monsterId = prop["value"].GetInt();
                                         }
                                     } else {
                                         LOGI("Db","Unexpected DataType.");
                                     }
                                 }
                             }
                         }

                         unsigned int gid = objectObj["gid"].GetInt() - firstGid;

                         if(monsterId -1 <objectAryCount) {
                             monsters[monsterId - 1] = new Monster(gid, new Property(lv,hp, atk, def,
                                                                                     agi), coin,
                                                                   exp);
                         }
                         else{
                             LOGI("Db","Invalid MonsterId.");
                         }
                     }
                 }
             }
        }

        delete[] tileSetFirstGids;
    }

public:

    Db(FileIO *fileIO, const char *fileName) {

        load(fileIO, fileName);
    }

    Monster* getMonsterById(unsigned int monsterId){
        if(monsterId > 0){
            return monsters[monsterId - 1];
        }
        return NULL;
    }

    virtual ~Db() {

        delete[] monsters;
    }
};


#endif //RPG2D_DB_H
