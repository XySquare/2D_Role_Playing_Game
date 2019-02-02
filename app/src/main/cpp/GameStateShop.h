//
// Created by Xyy on 2019/1/6.
//

#ifndef RPG2D_GAMESTATESHOP_H
#define RPG2D_GAMESTATESHOP_H


#include "Game.h"
#include "World.h"

class GameStateShop final : public Screen {

private:

    World *world;

    EventListener *eventListener;

    signed char pointer = -1;

    char index = 0;

    bool inBound(Touch e, short x, short y, unsigned short width, unsigned short height) {
        return e.x > x && e.x < x + width && e.y > y && e.y < y + height;
    }

public:

    GameStateShop(Game game, World *world, EventListener *eventListener) :
            Screen(game), world(world), eventListener(eventListener) {}

    void resume() override {

    }

    void update(float deltaTime) override {

        std::vector<Touch> &touchEvents = game.input->getTouchEvents();

        for (Touch e : touchEvents) {
            if (e.action == Action::TOUCH_DOWN) {
                if (pointer == -1) {
                    pointer = e.pointer;
                    if (e.x > 1216 && e.y < 64) {
                        index = 1;
                    } else if (inBound(e, 128, 160, 480, 200)) {
                        index = 2;
                    } else if (inBound(e, 672, 160, 480, 200)) {
                        index = 3;
                    } else if (inBound(e, 128, 424, 480, 200)) {
                        index = 4;
                    } else
                        index = 0;
                }
            } else if (e.action == Action::TOUCH_UP) {
                if (pointer == e.pointer) {
                    pointer = -1;
                    if (e.x > 1216 && e.y < 64 && index == 1)
                        eventListener->onReceive(Event::RUNNING, NULL);
                    else if (inBound(e, 128, 160, 480, 200) && index == 2) {
                        if (world->coin >= 25) {
                            world->coin -= 25;
                            world->player.prop->atk += 2;
                        } else {
                            eventListener->onReceive(NOTIFICATION, "Not enough coin");
                        }
                    } else if (inBound(e, 672, 160, 480, 200) && index == 3) {
                        if (world->coin >= 25) {
                            world->coin -= 25;
                            world->player.prop->def += 2;
                        } else {
                            eventListener->onReceive(NOTIFICATION, "Not enough coin");
                        }
                    } else if (inBound(e, 128, 424, 480, 200) && index == 4) {
                        if (world->coin >= 10) {
                            world->coin -= 10;
                            world->bag->potion_s++;
                        } else {
                            eventListener->onReceive(NOTIFICATION, "Not enough coin");
                        }
                    }
                    index = 0;
                    break;
                }
            }
        }
    }

    void present() override {

        SpriteBatcher *spriteBatcher = game.graphic;

        // Background
        spriteBatcher->beginBatch(0.f, 0.f, 0.f, .5f);
        spriteBatcher->drawSprite(0, 0, 1280, 720, TextureRegion(0, 0, 1, 1));
        spriteBatcher->endBatch();

        // UI
        spriteBatcher->beginBatch(Assets::ui);

        // Close
        spriteBatcher->drawSprite(1216, 0, 64, 64, TextureRegion(448, 64, 64, 64));

        // Title
        FontHelper::drawText(spriteBatcher, 604.75f, 96, "SHOP");

        // Coin
        spriteBatcher->drawSprite(918, 88, 48, 48, TextureRegion(0, 448, 64, 64));
        FontHelper::drawNumber(spriteBatcher, 1152, 96, world->coin);

        if (index == 2)
            spriteBatcher->drawSprite(128, 160, 480, 200, TextureRegion(448, 128, 64, 64));
        else
            spriteBatcher->drawSprite(128, 160, 480, 200, TextureRegion(384, 128, 64, 64));
        spriteBatcher->drawSprite(144, 176, 64, 64, TextureRegion(384, 448, 64, 64));
        FontHelper::drawText(spriteBatcher, 224, 176, "ATK +2");
        FontHelper::drawText(spriteBatcher, 224, 224, "25 G");

        if (index == 3)
            spriteBatcher->drawSprite(672, 160, 480, 200, TextureRegion(448, 128, 64, 64));
        else
            spriteBatcher->drawSprite(672, 160, 480, 200, TextureRegion(384, 128, 64, 64));
        spriteBatcher->drawSprite(688, 176, 64, 64, TextureRegion(448, 448, 64, 64));
        FontHelper::drawText(spriteBatcher, 768, 176, "DEF +2");
        FontHelper::drawText(spriteBatcher, 768, 224, "25 G");

        if (index == 4)
            spriteBatcher->drawSprite(128, 424, 480, 200, TextureRegion(448, 128, 64, 64));
        else
            spriteBatcher->drawSprite(128, 424, 480, 200, TextureRegion(384, 128, 64, 64));
        spriteBatcher->drawSprite(144, 440, 64, 64, TextureRegion(256, 448, 64, 64));
        FontHelper::drawText(spriteBatcher, 224, 440, "Potion (S)");
        FontHelper::drawText(spriteBatcher, 224, 488, "10 G");

        spriteBatcher->endBatch();
    }

    void onReceive(Event what, const void *arg) override {

    }
};


#endif //RPG2D_GAMESTATESHOP_H
