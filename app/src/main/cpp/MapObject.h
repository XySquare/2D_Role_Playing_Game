//
// Created by Xyy on 2018/12/21.
//

#ifndef RPG2D_MAPOBJECT_H
#define RPG2D_MAPOBJECT_H


#include <string>
#include "GameObject.h"
#include "Tile.h"

struct ObjectProperty {

    enum Type : unsigned char {
        INT = 0,
        STRING
    };

    const std::string name;

    const Type type;

    ObjectProperty(const std::string &name, const Type type) : name(name), type(type) {}
};

struct IntProperty : ObjectProperty {

    const int value;

    IntProperty(const std::string &name, const int value) : ObjectProperty(name, INT),
                                                            value(value) {}
};

struct StringProperty : ObjectProperty {

    const std::string value;

    StringProperty(const std::string &name, const std::string &value) : ObjectProperty(name,
                                                                                       STRING),
                                                                        value(value) {}
};

class MapObject : public GameObject {

public:

    unsigned int id;

    std::string type;

    std::string name;

    unsigned int width;

    unsigned int height;

    Tile *tile;

    unsigned char propertyCount;

    ObjectProperty **properties;

    //float rotation;

    //bool visible;

    MapObject(unsigned int id, float x, float y,
              const std::string &type, const std::string &name,
              unsigned int width, unsigned int height,
              Tile *tile,
              unsigned char propertyCount, ObjectProperty **properties)
            : GameObject(x, y), id(id),
              type(type), name(name),
              width(width), height(height),
              tile(tile),
              propertyCount(propertyCount), properties(properties) {}

    int getIntProperty(const char *name) {

        if(!propertyCount)
            return 0;
        for (unsigned char i = 0; i < propertyCount; i++) {
            ObjectProperty *objectProperty = properties[i];
            if (objectProperty->type == ObjectProperty::Type::INT && objectProperty->name == name) {
                return static_cast<IntProperty *>(objectProperty)->value;
            }
        }
        //Property not found
        return 0;
    }

    std::string getStringProperty(const char *name) {

        if(!propertyCount)
            return "";
        for (unsigned char i = 0; i < propertyCount; i++) {
            ObjectProperty *objectProperty = properties[i];
            if (objectProperty->type == ObjectProperty::Type::STRING && objectProperty->name == name) {
                return static_cast<StringProperty *>(objectProperty)->value;
            }
        }
        //Property not found
        return "";
    }

    virtual ~MapObject() {

        // tile will be delete in TileSet
        if(propertyCount > 0){
            for(int i = 0; i < propertyCount; ++i)
                delete properties[i];
            delete[] properties;
        }
    }
};


#endif //RPG2D_MAPOBJECT_H
