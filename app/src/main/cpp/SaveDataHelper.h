//
// Created by Xyy on 2018/12/31.
//

#ifndef RPG2D_SAVEDATAHELPER_H
#define RPG2D_SAVEDATAHELPER_H


#include "FileIO.h"
#include "World.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

class MapObjectSav {

public:

    unsigned int id;

    int x;

    int y;

    std::string type;

    std::string name;

    unsigned int width;

    unsigned int height;

    unsigned int gid;

    unsigned char propertyCount;

    ObjectProperty **properties;

    MapObjectSav() {}

    MapObjectSav(MapObject *mapObject) :
            id(mapObject->id),
            x(static_cast<int>(mapObject->position.x)),
            y(static_cast<int>(mapObject->position.y)),
            type(mapObject->type), name(mapObject->name),
            width(mapObject->width), height(mapObject->height),
            gid(static_cast<unsigned int>(mapObject->tile ? mapObject->tile->getGId(0) + 1 : 0)),
            propertyCount(mapObject->propertyCount) {

        if (propertyCount > 0) {
            ObjectProperty **objectProperties = new ObjectProperty *[propertyCount];
            for (unsigned char k = 0; k < propertyCount; k++) {
                ObjectProperty *objectProperty = mapObject->properties[k];
                if (objectProperty->type == ObjectProperty::Type::INT) {
                    objectProperties[k] = new IntProperty(objectProperty->name,
                                                          static_cast<IntProperty *>(objectProperty)->value);
                } else if (objectProperty->type == ObjectProperty::Type::STRING) {
                    objectProperties[k] = new StringProperty(objectProperty->name,
                                                             static_cast<StringProperty *>(objectProperty)->value);
                }
            }
            properties = objectProperties;
        } else {
            properties = NULL;
        }

    }

    virtual ~MapObjectSav() {

        if (propertyCount > 0) {
            for (unsigned char i = 0; i < propertyCount; ++i)
                delete properties[i];
            delete[] properties;
        }
    }
};

class ObjectLayerSav {

public:

    unsigned char id;

    unsigned int objectCount;

    MapObjectSav **objects;

    ObjectLayerSav() {}

    ObjectLayerSav(unsigned char id, unsigned int objectCount, MapObjectSav **const objects) : id(
            id), objectCount(
            objectCount), objects(objects) {}

    virtual ~ObjectLayerSav() {

        for (unsigned int i = 0; i < objectCount; ++i)
            delete objects[i];
        delete[] objects;
    }
};

class MapSav {

public:

    std::string id;

    const unsigned char layerCount;

    ObjectLayerSav **const layers;

    MapSav(const std::string &id, const unsigned char layerCount, ObjectLayerSav **const layers)
            : id(id),
              layerCount(layerCount),
              layers(layers) {}

    virtual ~MapSav() {

        for (unsigned char i = 0; i < layerCount; ++i)
            delete layers[i];
        delete[] layers;
    }
};

class SaveDataHelper {

private:

    static std::vector<MapSav *> mapSavs;

    static void put(MapSav *mapSav) {

        unsigned int size = static_cast<unsigned int>(mapSavs.size());
        unsigned int k;
        for (k = 0; k < size; k++) {
            if (mapSavs[k]->id == mapSav->id) {
                delete mapSavs[k];
                mapSavs[k] = mapSav;
                break;
            }
        }
        if (k == size)
            mapSavs.push_back(mapSav);
    }

    static MapSav *get(std::string &mapId) {

        for (unsigned int k = 0; k < mapSavs.size(); k++) {
            if (mapSavs[k]->id == mapId) {
                return mapSavs[k];
            }
        }
        return NULL;
    }

public:

    static void saveMap(std::string mapId, Map *map) {

        unsigned char objLayerCount = 0;
        unsigned char layerCount = map->layerCount;
        ObjectLayerSav **objectLayerSavsTemp = new ObjectLayerSav *[layerCount];
        for (unsigned char i = 0; i < layerCount; i++) {
            MapLayer *mapLayer = map->layers[i];
            if (mapLayer->layerType == MapLayer::OBJECT_LAYER) {
                ObjectLayer *objectLayer = (ObjectLayer *) mapLayer;
                unsigned int objectCount = objectLayer->objectCount;
                MapObjectSav **mapObjectSavs = new MapObjectSav *[objectCount];
                for (unsigned int j = 0; j < objectCount; j++) {
                    MapObject *object = objectLayer->objects[j];
                    mapObjectSavs[j] = new MapObjectSav(object);
                }
                objectLayerSavsTemp[objLayerCount++] = new ObjectLayerSav(objectLayer->id,
                                                                          objectLayer->objectCount,
                                                                          mapObjectSavs);
            }
        }
        // Copy objects
        ObjectLayerSav **objectLayerSavs = new ObjectLayerSav *[objLayerCount];
        for (unsigned char j = 0; j < objLayerCount; j++) {
            objectLayerSavs[j] = objectLayerSavsTemp[j];
        }
        delete[] objectLayerSavsTemp;

        put(new MapSav(mapId, objLayerCount, objectLayerSavs));
    }

    static bool save(FileIO *fileIO, World *world) {

        saveMap(world->curMap, world->map);

        Player &player = world->player;
        Bag *bag = world->bag;
        Map *map = world->map;

        using namespace rapidjson;

        Document worldObj;
        worldObj.SetObject();
        Document::AllocatorType &allocator = worldObj.GetAllocator();

        // Bag
        Value bagObj(kObjectType);
        // Bag/Key
        Value keyAry(kArrayType);
        for (unsigned char i = 0; i < 3; i++)
            keyAry.PushBack(world->bag->key[i], allocator);
        bagObj.AddMember("key", keyAry, allocator);
        worldObj.AddMember("bag", bagObj, allocator);

        // Coin
        worldObj.AddMember("coin", world->coin, allocator);

        // Current Map
        worldObj.AddMember("curMap", Value(world->curMap.c_str(), allocator).Move(), allocator);

        // Exp
        worldObj.AddMember("exp", world->exp, allocator);

        // Player
        Value playerObj(kObjectType);
        // Player/Position
        Value playerPositionObj(kObjectType);
        playerPositionObj.AddMember("x", player.position.x, allocator);
        playerPositionObj.AddMember("y", player.position.y, allocator);
        playerObj.AddMember("position", playerPositionObj, allocator);
        // Player/Property
        Value playerPropertyObj(kObjectType);
        playerPropertyObj.AddMember("agi", player.prop->agi, allocator);
        playerPropertyObj.AddMember("atk", player.prop->atk, allocator);
        playerPropertyObj.AddMember("def", player.prop->def, allocator);
        playerPropertyObj.AddMember("hp", player.prop->hp, allocator);
        playerPropertyObj.AddMember("maxHp", player.prop->maxHp, allocator);
        playerObj.AddMember("property", playerPropertyObj, allocator);
        worldObj.AddMember("player", playerObj, allocator);

        // Maps
        Value mapsObj(kObjectType);
        for (MapSav *m:mapSavs) {
            std::string mapId = m->id;
            // Layers
            Value layerAry(kArrayType);
            for (unsigned char i = 0; i < m->layerCount; i++) {
                // Layer
                Value layerObj(kObjectType);

                Value objectAry(kArrayType);
                ObjectLayerSav *layerSav = m->layers[i];
                for (int j = 0; j < layerSav->objectCount; j++) {
                    MapObjectSav *objectSav = layerSav->objects[j];
                    Value mapObjectObj(kObjectType);
                    mapObjectObj.AddMember("id", objectSav->id, allocator);
                    mapObjectObj.AddMember("x", objectSav->x, allocator);
                    mapObjectObj.AddMember("y", objectSav->y, allocator);
                    mapObjectObj.AddMember("type", Value(objectSav->type.c_str(), allocator).Move(),
                                           allocator);
                    mapObjectObj.AddMember("name", Value(objectSav->name.c_str(), allocator).Move(),
                                           allocator);
                    mapObjectObj.AddMember("width", objectSav->width, allocator);
                    mapObjectObj.AddMember("height", objectSav->height, allocator);
                    mapObjectObj.AddMember("gid", objectSav->gid, allocator);

                    if (objectSav->propertyCount > 0) {
                        Value propertiesObj(kObjectType);

                        ObjectProperty **objectProperties = objectSav->properties;
                        for (unsigned char k = 0; k < objectSav->propertyCount; k++) {
                            ObjectProperty *objectProperty = objectProperties[k];
                            if (objectProperty->type == ObjectProperty::Type::INT) {
                                propertiesObj.AddMember(
                                        Value(objectProperty->name.c_str(), allocator).Move(),
                                        static_cast<IntProperty *>(objectProperty)->value,
                                        allocator);
                            } else if (objectProperty->type == ObjectProperty::Type::STRING) {
                                propertiesObj.AddMember(
                                        Value(objectProperty->name.c_str(), allocator).Move(),
                                        Value(static_cast<StringProperty *>(objectProperty)->value.c_str(),
                                              allocator).Move(), allocator);
                            }
                        }

                        mapObjectObj.AddMember("properties", propertiesObj, allocator);
                    }

                    objectAry.PushBack(mapObjectObj, allocator);
                }

                layerObj.AddMember("id", layerSav->id, allocator);
                layerObj.AddMember("objects", objectAry, allocator);

                layerAry.PushBack(layerObj, allocator);
            }

            mapsObj.AddMember(Value(mapId.c_str(), allocator).Move(), layerAry, allocator);
        }
        worldObj.AddMember("maps", mapsObj, allocator);

        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        worldObj.Accept(writer);

        const char *str = buffer.GetString();

        FILE *file = fileIO->writeFile("SaveData");
        if (!file) {
            return false;
        }
        size_t count = buffer.GetSize();
        if (fwrite(str, 1, count, file) != count) {
            fclose(file);
            return false;
        }
        fclose(file);

        return true;
    }

    static void loadMap(std::string &mapId, Map *map) {

        MapSav *mapSav = get(mapId);
        if (!mapSav)
            return;

        unsigned char layerCount = map->layerCount;
        for (unsigned char i = 0; i < layerCount; i++) {
            MapLayer *mapLayer = map->layers[i];
            if (mapLayer->layerType == MapLayer::OBJECT_LAYER) {
                ObjectLayer *objectLayer = (ObjectLayer *) mapLayer;
                unsigned char layerId = objectLayer->id;

                //Get a saved layer matching the ID
                ObjectLayerSav *objectLayerSav = NULL;
                for (unsigned char j = 0; j < mapSav->layerCount; j++) {
                    if (mapSav->layers[j]->id == layerId) {
                        objectLayerSav = mapSav->layers[j];
                        break;
                    }
                }
                if (!objectLayerSav)
                    return;
                // Found

                TileSet *tileSet = map->tileSets[objectLayer->tileSetIndex];

                unsigned int objectCount = objectLayerSav->objectCount;
                MapObject **mapObjects = new MapObject *[objectCount];
                for (unsigned int l = 0; l < objectCount; l++) {
                    MapObjectSav *mapObjectSav = objectLayerSav->objects[l];

                    unsigned int id = mapObjectSav->id;
                    unsigned int objWidth = mapObjectSav->width;
                    unsigned int objHeight = mapObjectSav->height;
                    int x = mapObjectSav->x;
                    int y = mapObjectSav->y;

                    Tile *tile = NULL;

                    unsigned char propertyCount = mapObjectSav->propertyCount;
                    ObjectProperty **objectProperties = NULL;
                    if (propertyCount > 0) {
                        objectProperties = new ObjectProperty *[propertyCount];
                        for (unsigned char m = 0; m < propertyCount; m++) {
                            ObjectProperty *objectProperty = mapObjectSav->properties[m];
                            if (objectProperty->type == ObjectProperty::Type::INT) {
                                objectProperties[m] = new IntProperty(objectProperty->name,
                                                                      static_cast<IntProperty *>(objectProperty)->value);
                            } else if (objectProperty->type == ObjectProperty::Type::STRING) {
                                objectProperties[m] = new StringProperty(objectProperty->name,
                                                                         static_cast<StringProperty *>(objectProperty)->value);
                            }
                        }
                    }

                    // Tile Object
                    if (mapObjectSav->gid > 0) {
                        unsigned int gid = mapObjectSav->gid - 1;
                        tile = tileSet->tiles[gid];
                    }

                    mapObjects[l] = new MapObject(id, x, y,
                                                  mapObjectSav->type, mapObjectSav->name,
                                                  objWidth, objHeight,
                                                  tile,
                                                  propertyCount,
                                                  objectProperties);

                }

                for (unsigned int m = 0; m < objectCount; ++m)
                    delete objectLayer->objects[m];
                delete[] objectLayer->objects;

                objectLayer->objectCount = objectCount;
                objectLayer->objects = mapObjects;
            }
        }
    }

    static bool load(FileIO *fileIO, World *world) {

        FILE *file = fileIO->readFile("SaveData");

        if (!file) {
            return false;
        }

        fseek(file, 0, SEEK_END);
        unsigned long length = static_cast<unsigned long>(ftell(file));
        fseek(file, 0, SEEK_SET);
        char *buffer = static_cast<char *>(malloc(length + 1));
        if(!buffer){
            return false;
        }
        if(fread(buffer, 1, length, file) != length){
            fclose(file);
            return false;
        }
        fclose(file);
        buffer[length] = '\0';

        Player &player = world->player;
        Bag *bag = world->bag;
        Map *map = world->map;

        using namespace rapidjson;

        Document worldObj;
        worldObj.Parse(buffer);
        Value &bagObj = worldObj["bag"];
        Value &keyAry = bagObj["key"];
        for (unsigned char i = 0; i < 3; i++)
            world->bag->key[i] = (unsigned short) keyAry[i].GetInt();
        world->coin = static_cast<unsigned int>(worldObj["coin"].GetInt());
        world->nextMap = worldObj["curMap"].GetString();
        world->exp = static_cast<unsigned int>(worldObj["exp"].GetInt());
        Value &playerObj = worldObj["player"];
        Value &playerPositionObj = playerObj["position"];
        world->nextPosition.x = playerPositionObj["x"].GetFloat();
        world->nextPosition.y = playerPositionObj["y"].GetFloat();
        Value &playerPropertyObj = playerObj["property"];
        player.prop->agi = playerPropertyObj["agi"].GetInt();
        player.prop->atk = playerPropertyObj["atk"].GetInt();
        player.prop->def = playerPropertyObj["def"].GetInt();
        player.prop->hp = playerPropertyObj["hp"].GetInt();
        player.prop->maxHp = playerPropertyObj["maxHp"].GetInt();
        for (MapSav *m:mapSavs) {
            delete m;
        }
        mapSavs.clear();
        Value &mapsObj = worldObj["maps"];
        for (Value::ConstMemberIterator itr = mapsObj.MemberBegin();
             itr != mapsObj.MemberEnd(); ++itr) {
            const char *mapId = itr->name.GetString();

            Value &layerAry = mapsObj[mapId];
            unsigned char layerAryCount = static_cast<unsigned char>(layerAry.Size());
            ObjectLayerSav **layerSavs = new ObjectLayerSav *[layerAryCount];
            for (unsigned char i = 0; i < layerAryCount; i++) {
                ObjectLayerSav *layerSav = new ObjectLayerSav();

                Value &layerObj = layerAry[i];
                layerSav->id = static_cast<unsigned char>(layerObj["id"].GetInt());
                Value &objectAry = layerObj["objects"];
                SizeType objectAryCount = objectAry.Size();
                MapObjectSav **objectSavs = new MapObjectSav *[objectAryCount];
                for (unsigned int j = 0; j < objectAryCount; j++) {
                    MapObjectSav *objectSav = new MapObjectSav();

                    Value &mapObjectObj = objectAry[j];
                    objectSav->id = static_cast<unsigned int>(mapObjectObj["id"].GetInt());
                    objectSav->x = mapObjectObj["x"].GetInt();
                    objectSav->y = mapObjectObj["y"].GetInt();
                    objectSav->type = mapObjectObj["type"].GetString();
                    objectSav->name = mapObjectObj["name"].GetString();
                    objectSav->width = static_cast<unsigned int>(mapObjectObj["width"].GetInt());
                    objectSav->height = static_cast<unsigned int>(mapObjectObj["height"].GetInt());
                    objectSav->gid = static_cast<unsigned int>(mapObjectObj["gid"].GetInt());

                    if (mapObjectObj.HasMember("properties")) {
                        Value &propertyObj = mapObjectObj["properties"];
                        unsigned char propertyCount = static_cast<unsigned char>(propertyObj.MemberCount());
                        ObjectProperty **objectProperties = new ObjectProperty *[propertyCount];
                        unsigned char k = 0;
                        for (Value::ConstMemberIterator itr2 = propertyObj.MemberBegin();
                             itr2 != propertyObj.MemberEnd(); ++itr2) {
                            const char *propertyName = itr2->name.GetString();
                            Value &propertyValue = propertyObj[propertyName];
                            if (propertyValue.IsInt()) {
                                objectProperties[k++] = new IntProperty(propertyName,
                                                                        propertyValue.GetInt());
                            } else if (propertyValue.IsString()) {
                                objectProperties[k++] = new StringProperty(propertyName,
                                                                           propertyValue.GetString());
                            }
                        }

                        objectSav->propertyCount = propertyCount;
                        objectSav->properties = objectProperties;
                    } else {
                        objectSav->propertyCount = 0;
                        objectSav->properties = NULL;
                    }

                    objectSavs[j] = objectSav;
                }
                layerSav->objectCount = objectAryCount;
                layerSav->objects = objectSavs;

                layerSavs[i] = layerSav;
            }

            mapSavs.push_back(new MapSav(mapId, layerAryCount, layerSavs));
        }
        free(buffer);

        world->loading(fileIO);

        loadMap(world->curMap, world->map);

        return true;
    }
};

std::vector<MapSav *> SaveDataHelper::mapSavs;

#endif //RPG2D_SAVEDATAHELPER_H
