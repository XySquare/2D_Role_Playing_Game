//
// Created by Xyy on 2018/12/26.
//

#ifndef RPG2D_GAMESTATERUNNING_H
#define RPG2D_GAMESTATERUNNING_H


#include <math.h>

#include "Screen.h"
#include "World.h"
#include "FontHelper.h"

class GameStateRunning final : public Screen {

private:

    World *world;

    EventListener *eventListener;

    float controllerX = 160;

    float controllerY = 560;

    signed char controllerPointer = -1;

    Vector2 playerVelocity;

    unsigned char door = 0;

    signed char pointer = -1;

    unsigned char index = 0;

    bool updateController(Touch event, Vector2 &vector) {

        const int LCtrlX = 160;
        const int LCtrlY = 560;
        const int maxR = 100;
        const int minR = 25;
        int dx = event.x - LCtrlX;
        int dy = event.y - LCtrlY;
        float length = (float) sqrt(dx * dx + dy * dy);
        if (event.action == Action::TOUCH_DOWN) {
            if (length < maxR && controllerPointer == -1) {
                controllerPointer = event.pointer;
                controllerX = event.x;
                controllerY = event.y;
                if (length > minR) {
                    vector.x = (float) dx / length;
                    vector.y = (float) dy / length;
                }
                return true;
            }
        } else if (event.action == Action::TOUCH_DRAGGED) {
            if (event.pointer == controllerPointer) {
                if (length > minR) {
                    vector.x = (float) dx / length;
                    vector.y = (float) dy / length;
                } else {
                    vector.x = 0;
                    vector.y = 0;
                }
                if (length < maxR) {
                    controllerX = event.x;
                    controllerY = event.y;
                } else {
                    controllerX = LCtrlX + vector.x * maxR;
                    controllerY = LCtrlY + vector.y * maxR;
                }
                return true;
            }
        } else if (event.action == Action::TOUCH_UP) {
            if (event.pointer == controllerPointer) {
                controllerPointer = -1;
                vector.x = 0;
                vector.y = 0;
                controllerX = LCtrlX;
                controllerY = LCtrlY;
                return true;
            }
        }
        return false;
    }

    void showMonsterInfo(Touch &e) {
        unsigned char layerCount = world->map->layerCount;
        for (unsigned char i = 0; i < layerCount; i++) {
            MapLayer *mapLayer = world->map->layers[i];
            if (mapLayer->layerType == MapLayer::OBJECT_LAYER) {
                ObjectLayer *objectLayer = (ObjectLayer *) mapLayer;
                for (unsigned int j = 0; j < objectLayer->objectCount; j++) {
                    MapObject *object = objectLayer->objects[j];
                    if (object->type == "monster") {
                        float offsetX = 640 - world->player.position.x -
                                        Player::PLAYER_WIDTH / 2;
                        float offsetY = 360 - world->player.position.y -
                                        Player::PLAYER_HEIGHT / 2;
                        // Adjust offsets
                        int mapWidth = world->map->width * world->map->tileWidth;
                        if (mapWidth < 1280)
                            offsetX = (1280 - mapWidth) / 2.f;
                        else if (offsetX > 0) offsetX = 0;
                        else if (offsetX < 1280 - mapWidth)
                            offsetX = 1280 - mapWidth;
                        int mapHeight = world->map->height * world->map->tileHeight;
                        if (mapHeight < 720)
                            offsetX = (720 - mapHeight) / 2.f;
                        else if (offsetY > 0) offsetY = 0;
                        else if (offsetY < 720 - mapHeight)
                            offsetY = 720 - mapHeight;
                        if (inBound(e, object->position.x + offsetX,
                                    object->position.y + offsetY, object->width,
                                    object->height)) {
                            int arg = object->getIntProperty("a");
                            Vector2 v(arg, e.x);
                            eventListener->onReceive(Event::MONSTER_INFO, &v);
                            break;
                        }
                    }
                }
            }
        }
    }

    bool inBound(Touch e, unsigned short x, unsigned short y, unsigned short r) {
        int dx = x - e.x;
        int dy = y - e.y;
        return dx * dx + dy * dy < r * r;
    }

    bool inBound(Touch e, float x, float y, unsigned int width, unsigned int height) {
        return e.x > x && e.x < x + width && e.y > y && e.y < y + height;
    }

public:

    GameStateRunning(Game game, World *world, EventListener *eventListener) :
            Screen(game), world(world), eventListener(eventListener),
            playerVelocity(0, 0) {}

    virtual void resume() override {

    }

    virtual void update(float deltaTime) override {

        world->update(deltaTime);

        std::vector<Touch> &touchEvents = game.input->getTouchEvents();

        for (Touch e : touchEvents) {
            if (updateController(e, playerVelocity)) {
                world->setPlayerVelocity(playerVelocity);
            } else if (e.action == Action::TOUCH_DOWN) {
                if (pointer == -1) {
                    pointer = e.pointer;
                    if (e.x > 1200 && e.y < 80) {
                        index = 1;
                    } else if (door && inBound(e, 1136, 576, 80)) {
                        index = 2;
                    } else {
                        showMonsterInfo(e);
                    }
                }
            } else if (e.action == Action::TOUCH_UP) {
                if (pointer == e.pointer) {
                    pointer = -1;
                    if (e.x > 1200 && e.y < 80 && index == 1) {
                        eventListener->onReceive(Event::PAUSE, NULL);
                    } else if (door && inBound(e, 1136, 576, 80) && index == 2) {
                        if (world->bag->key[door - 1] > 0) {
                            world->bag->key[door - 1]--;
                            world->removeMonster();
                            door = 0;
                        }
                        else {
                            eventListener->onReceive(NOTIFICATION, "No Keys");
                        }
                    }
                }
            }
        }
    }

    virtual void present() override {

        SpriteBatcher *spriteBatcher = game.graphic;

        spriteBatcher->beginBatch(Assets::ui);

        // Menu
        spriteBatcher->drawSprite(1216, 0, 64, 64,
                                  TextureRegion(448 / 512.f, 0, 64 / 512.f, 64 / 512.f));

        // Controller
        spriteBatcher->drawSprite(64, 464, 192, 192,
                                  TextureRegion(0, 0, 192 / 512.f, 192 / 512.f));
        spriteBatcher->drawSprite(controllerX - 80, controllerY - 80, 160, 160,
                                  TextureRegion(192 / 512.f, 0, 160 / 512.f, 160 / 512.f));

        // Bottom-right button
        if (door) {
            spriteBatcher->drawSprite(1056, 496, 160, 160,
                                      TextureRegion(192 / 512.f, 0, 160 / 512.f, 160 / 512.f));

            spriteBatcher->drawSprite(1072, 512, 128, 128,
                                      TextureRegion(64*door / 512.f, 448 / 512.f, 64 / 512.f, 64 / 512.f));
            FontHelper::drawNumber(spriteBatcher, 1168, 608, world->bag->key[door-1]);
        }

        spriteBatcher->endBatch();
    }

    void onReceive(Event what, const void *arg) override {

        if (what == Event::DOOR) {

            door = static_cast<unsigned char>(arg == NULL ? 0 : *((int *) arg));
        }
    }

    virtual ~GameStateRunning() {

    }
};


#endif //RPG2D_GAMESTATERUNNING_H
