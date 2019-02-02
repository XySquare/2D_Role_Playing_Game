//
// Created by Xyy on 2019/1/13.
//

#ifndef RPG2D_GAMESTATETITLE_H
#define RPG2D_GAMESTATETITLE_H


#include "Screen.h"
#include "World.h"
#include "SaveDataHelper.h"

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

    void resume() override {

    }

    void update(float deltaTime) override {

        std::vector<Touch> &touchEvents = game.input->getTouchEvents();

        for (Touch e : touchEvents) {
            if (e.action == Action::TOUCH_DOWN) {
                if (pointer == -1) {
                    pointer = e.pointer;
                    if (inBound(e, 145, 534, 228, 58)) {
                        index = 2;
                    } else if (inBound(e, 501, 534, 228, 58)) {
                        index = 3;
                    } else if (inBound(e, 907, 534, 228, 58)) {
                        index = 4;
                    } else
                        index = 0;
                }
            } else if (e.action == Action::TOUCH_UP) {
                if (pointer == e.pointer) {
                    pointer = -1;
                    if (inBound(e, 145, 534, 228, 58) && index == 2) {
                        // New Game
                        eventListener->onReceive(RUNNING, NULL);
                    } else if (inBound(e, 501, 534, 228, 58) && index == 3) {
                        // Load
                        if (SaveDataHelper::load(game.fileIO, world)) {
                            eventListener->onReceive(TRANSFER, NULL);
                        } else {
                            eventListener->onReceive(NOTIFICATION, "No Data");
                        }
                    } else if (inBound(e, 907, 534, 228, 58) && index == 4) {
                        // Help
                        eventListener->onReceive(WEBVIEW, "file:///android_asset/help.html");
                    }
                    index = 0;
                    break;
                }
            }
        }
    }

    void present() override {

        SpriteBatcher *spriteBatcher = game.graphic;

        spriteBatcher->beginBatch(Assets::ui);

        // Title
        spriteBatcher->drawSprite(414, 124, 452, 236, TextureRegion(0, 640, 226, 118));

        // NEW GAME
        spriteBatcher->drawSprite(145, (index == 2 ? 538 : 534), 228, 58,
                                  TextureRegion(240, 640, 114, 29));

        // LOAD
        spriteBatcher->drawSprite(501, (index == 3 ? 538 : 534), 278, 58,
                                  TextureRegion(240, 672, 139, 29));

        // HELP
        spriteBatcher->drawSprite(907, (index == 4 ? 538 : 534), 278, 58,
                                  TextureRegion(240, 704, 139, 29));

        spriteBatcher->endBatch();
    }

    void onReceive(Event what, const void *arg) override {

    }
};


#endif //RPG2D_GAMESTATETITLE_H
