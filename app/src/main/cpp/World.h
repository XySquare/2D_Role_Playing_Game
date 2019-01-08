//
// Created by Xyy on 2018/12/19.
//

#ifndef RPG2D_WORLD_H
#define RPG2D_WORLD_H


#include "Player.h"
#include "MapLoader.h"
#include "Assets.h"
#include "EventListener.h"
#include "Bag.h"

class World final {

private:

    EventListener *eventListener;

    MapObject *monster = NULL;

    bool isAccessible(int landform) {
        return landform == 0;
    }

public:

    enum State : unsigned char {
        NORMAL,
        LOADING
    };

    State state;

    std::string curMap;

    std::string nextMap;

    Vector nextPosition;

    Map *map = NULL;

    Player player;

    float timer = 0;

    Bag *bag;

    unsigned int coin = 99999;

    unsigned int exp = 99999;

    World(EventListener *eventListener) : player(200, 200), nextPosition(0,0), eventListener(eventListener), bag(new Bag()) {

        state = LOADING;
        nextMap = "tiled_map00.json";
        nextPosition.x = 1792;
        nextPosition.y = 640;
        LOGI("World Created.");
    }

    void setPlayerVelocity(Vector v) {

        player.velocity = v;
    }

    void loading(FileIO *fileIO) {

        state = LOADING;
        if(map)
            delete map;
        curMap = nextMap;
        MapLoader mapLoader;
        map = mapLoader.loadMap(fileIO, nextMap.c_str());
        Assets::load(map);
        player.position.x = nextPosition.x;
        player.position.y = nextPosition.y;
        nextMap.clear();
        state = NORMAL;
    }

    void update(float deltaTime) {

        if(state != NORMAL)
            return;

        timer += deltaTime;
        if (timer > 12.f) {
            timer -= 12.f;
        }

        float prevRecttop = player.position.y;
        float prevRectbottom = player.position.y + Player::PLAYER_HEIGHT;
        float prevRectleft = player.position.x;
        float prevRectright = player.position.x + Player::PLAYER_WIDTH;

        player.update(deltaTime);

        const int tileWidth = 64;

        /*
        * Calculate player current position
        */
        float curRecttop = player.position.y;
        float curRectbottom = player.position.y + Player::PLAYER_HEIGHT;
        float curRectleft = player.position.x;
        float curRectright = player.position.x + Player::PLAYER_WIDTH;

        /*
          Collision Detection
        */

        float offsetX = 0;
        float offsetY = 0;

        const unsigned char *layer = map->collisionLayer;
        int width = map->width;

        if (layer != NULL) {
            int minIndexY = (int) (curRecttop / tileWidth);
            int maxIndexY = (int) (curRectbottom / tileWidth);
            int minIndexX = (int) (curRectleft / tileWidth);
            int maxIndexX = (int) (curRectright / tileWidth);

            if (curRecttop > prevRecttop) {
                for (int ix = minIndexX; ix <= maxIndexX; ix++) {
                    if (!isAccessible(layer[width * maxIndexY + ix]) &&
                        isAccessible(layer[width * (maxIndexY - 1) + ix])) {
                        int edge = maxIndexY * tileWidth;
                        if (curRectbottom >= edge && prevRectbottom <= edge) {
                            offsetY = edge - curRectbottom;
                            break;
                        }
                    }
                }
            } else {
                for (int ix = minIndexX; ix <= maxIndexX; ix++) {
                    if (!isAccessible(layer[width * minIndexY + ix]) &&
                        isAccessible(layer[width * (minIndexY + 1) + ix])) {
                        int edge = (minIndexY + 1) * tileWidth;
                        if (curRecttop <= edge && prevRecttop >= edge) {
                            offsetY = edge - curRecttop;
                            break;
                        }
                    }
                }
            }

            if (curRectleft > prevRectleft) {
                for (int iy = minIndexY; iy <= maxIndexY; iy++) {
                    if (!isAccessible(layer[width * iy + maxIndexX]) &&
                        isAccessible(layer[width * iy + maxIndexX - 1])) {
                        int edge = maxIndexX * tileWidth;
                        if (curRectright >= edge && prevRectright <= edge) {
                            offsetX = edge - curRectright;
                            break;
                        }
                    }
                }
            } else {
                for (int iy = minIndexY; iy <= maxIndexY; iy++) {
                    if (!isAccessible(layer[width * iy + minIndexX]) &&
                        isAccessible(layer[width * iy + minIndexX + 1])) {
                        int edge = (minIndexX + 1) * tileWidth;
                        if (curRectleft <= edge && prevRectleft >= edge) {
                            offsetX = edge - curRectleft;
                            break;
                        }
                    }
                }
            }
        }

        player.position.x += offsetX;
        player.position.y += offsetY;

        /**
         * Interact with map objects
         */
        int layerCount = map->layerCount;
        for (int i = 0; i < layerCount; i++) {
            MapLayer *mapLayer = map->layers[i];

            if (mapLayer->layerType == MapLayer::OBJECT_LAYER) {

                ObjectLayer *objectLayer = (ObjectLayer *) mapLayer;

                for (int j = 0; j < objectLayer->objectCount; j++) {
                    MapObject *object = objectLayer->objects[j];

                    float top = player.position.y;
                    float bottom = player.position.y + Player::PLAYER_HEIGHT;
                    float left = player.position.x;
                    float right = player.position.x + Player::PLAYER_WIDTH;

                    float top2 = object->position.y;
                    float bottom2 = object->position.y + object->height;
                    float left2 = object->position.x;
                    float right2 = object->position.x + object->width;

                    if (left < right2 && right > left2 && top < bottom2 && bottom > top2) {
                        if (right2 - left < abs(left2 - right)) {
                            offsetX = right2 - left;
                        } else {
                            offsetX = left2 - right;
                        }
                        if (bottom2 - top < abs(top2 - bottom)) {
                            offsetY = bottom2 - top;
                        } else {
                            offsetY = top2 - bottom;
                        }
                        if (abs(offsetX) < abs(offsetY)) {
                            offsetY = 0;
                        } else {
                            offsetX = 0;
                        }
                        player.position.x += offsetX;
                        player.position.y += offsetY;
                        onUpdate(objectLayer, object);
                    }
                }
            }
        }

        if (monster) {

            MapObject *object = monster;

            float top = player.position.y;
            float bottom = player.position.y + Player::PLAYER_HEIGHT;
            float left = player.position.x;
            float right = player.position.x + Player::PLAYER_WIDTH;

            float top2 = object->position.y - 2;
            float bottom2 = object->position.y + object->height + 2;
            float left2 = object->position.x - 2;
            float right2 = object->position.x + object->width + 2;

            if (!(left < right2 && right > left2 && top < bottom2 && bottom > top2)) {

                monster = NULL;
                eventListener->onEvent(Event::DOOR, 0);
            }
        }
    }

    void onUpdate(ObjectLayer *objectLayer, MapObject *object) {

        if (object->type == "monster") {

            monster = object;
            eventListener->onEvent(Event::BATTLE, object->getIntProperty("monsterId"));
        } else if (object->type == "item") {

            eventListener->onEvent(Event::ITEM, object->getIntProperty("itemId"));
            objectLayer->remove(object);
        } else if (object->type == "door") {

            monster = object;
            eventListener->onEvent(Event::DOOR, object->getIntProperty("doorType"));
        } else if (object->type == "transfer") {

            //state = PENDING;
            nextMap = object->getStringProperty("a");
            nextPosition.x = object->getIntProperty("x");
            nextPosition.y = object->getIntProperty("y");
            eventListener->onEvent(Event::TRANSFER, 0);
        } else if (object->type == "shop") {

            eventListener->onEvent(Event::SHOP, object->getIntProperty("a"));
        } else{
            LOGI("Object Type: %s",object->type.c_str());
        }
    }

    float abs(float x) {

        return x < 0 ? -x : x;
    }

    float min(float x, float y) {

        return x > y ? y : x;
    }

    void removeMonster() {
        if (monster) {
            // Remove Object
            int layerCount = map->layerCount;
            for (int i = 0; i < layerCount; i++) {
                MapLayer *mapLayer = map->layers[i];

                if (mapLayer->layerType == MapLayer::OBJECT_LAYER) {

                    ObjectLayer *objectLayer = (ObjectLayer *) mapLayer;

                    objectLayer->remove(monster);
                }
            }
            monster = NULL;
        }
    }

    ~World() {

        delete map;
        delete bag;
    }
};


#endif //RPG2D_WORLD_H
