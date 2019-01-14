//
// Created by Xyy on 2019/1/13.
//

#ifndef RPG2D_GAMESTATETITLE_H
#define RPG2D_GAMESTATETITLE_H


#include "Screen.h"
#include "World.h"
#include "SaveDataHelper.h"
#include "FontHelper.h"

class GameStateTitle final : public Screen {

private:

    World *world;

    EventListener *eventListener;

    signed char pointer = -1;

    unsigned char index = 0;

    bool inBound(Touch e, short x, short y, unsigned short width, unsigned short height) {
        return e.x > x && e.x < x + width && e.y > y && e.y < y + height;
    }

public:
    GameStateTitle(const Game &game, World *world,
                   EventListener *eventListener) :
            Screen(game), world(world),
            eventListener(eventListener) {}

    void onReceive(Event what, const void *arg) override {

    }

    void resume() override {

    }

    void update(float deltaTime) override {

        std::vector<Touch> &touchEvents = game.input->getTouchEvents();

        for (Touch e : touchEvents) {
            LOGI("GameStateTitle", "%d,%d", e.x, e.y);
            if (e.action == Action::TOUCH_DOWN) {
                if (pointer == -1) {
                    pointer = e.pointer;
                    if (inBound(e, 814, 576, 192, 96)) {
                        index = 2;
                    } else if (inBound(e, 1024, 576, 192, 96)) {
                        index = 3;
                    } else
                        index = 0;
                }
            } else if (e.action == Action::TOUCH_UP) {
                if (pointer == e.pointer) {
                    pointer = -1;
                    if (inBound(e, 814, 576, 192, 96) && index == 2) {
                        // New Game
                        eventListener->onReceive(RUNNING, NULL);
                    } else if (inBound(e, 1024, 576, 192, 96) && index == 3) {
                        // Load
                        if (SaveDataHelper::load(game.fileIO, world)) {
                            eventListener->onReceive(TRANSFER, NULL);
                        }
                    }
                    index = 0;
                    break;
                }
            }
        }
    }

    void present() override {

        // Background
        /*spriteBatcher->beginBatch(Assets::unitTexture);
        glVertexAttrib4f(GRAPHIC_COLOR_HANDEL, 0.f, 0.f, 0.f, .5f);
        spriteBatcher->drawSprite(0, 0, 1280, 720, TextureRegion(0, 0, 1, 1));
        spriteBatcher->endBatch();*/

        SpriteBatcher *spriteBatcher = game.graphic;

        spriteBatcher->beginBatch(Assets::ui);

        // LOAD
        spriteBatcher->drawSprite(1024, 576, 192, 96,
                                  TextureRegion((index == 3 ? 448 : 384) / 512.f, 128 / 512.f,
                                                64 / 512.f,
                                                64 / 512.f));
        FontHelper::drawText(spriteBatcher, 1104, 612, "LOAD");

        // NEW GAME
        spriteBatcher->drawSprite(814, 576, 192, 96,
                                  TextureRegion((index == 2 ? 448 : 384) / 512.f, 128 / 512.f,
                                                64 / 512.f,
                                                64 / 512.f));
        FontHelper::drawText(spriteBatcher, 894, 612, "NEW");

        spriteBatcher->endBatch();
    }

};


#endif //RPG2D_GAMESTATETITLE_H
