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
                    if (e.x > 1216 && e.y < 64){
                        index = 1;
                    }
                    else if (inBound(e, 64 + 64, 64 + 96, 544 - 64, 200)){
                        index = 2;
                    }
                    else if (inBound(e, 672, 64 + 96, 544 - 64, 200)){
                        index = 3;
                    }
                    else if (inBound(e, 64 + 64, 64 + 96 + 200 + 64, 544 - 64, 200)){
                        index = 4;
                    }
                    else
                        index = 0;
                }
            } else if (e.action == Action::TOUCH_UP) {
                if (pointer == e.pointer) {
                    pointer = -1;
                    if (e.x > 1216 && e.y < 64 && index == 1)
                        eventListener->onReceive(Event::RUNNING, NULL);
                    else if (inBound(e, 64 + 64, 64 + 96, 544 - 64, 200) && index == 2) {
                        if (world->coin >= 25) {
                            world->coin -= 25;
                            world->player.prop->atk += 2;
                        }
                        else{
                            eventListener->onReceive(NOTIFICATION, "Not enough coin");
                        }
                    } else if (inBound(e, 672, 64 + 96, 544 - 64, 200) && index == 3) {
                        if (world->coin >= 25) {
                            world->coin -= 25;
                            world->player.prop->def += 2;
                        }
                        else{
                            eventListener->onReceive(NOTIFICATION, "Not enough coin");
                        }
                    } else if (inBound(e, 64 + 64, 64 + 96 + 200 + 64, 544 - 64, 200) && index == 4) {
                        if (world->coin >= 10) {
                            world->coin -= 10;
                            world->bag->potion_s++;
                        }
                        else{
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
        spriteBatcher->drawSprite(1216, 0, 64, 64,
                                  TextureRegion(448 / 512.f, 64 / 512.f, 64 / 512.f, 64 / 512.f));

        // Title
        FontHelper::drawText(spriteBatcher, 604.75f, 64 + 32, "SHOP");

        // Coin
        spriteBatcher->drawSprite(1072 - 90-64, 64 + 32-8, 48, 48,
                                  TextureRegion(0 / 512.f, (448) / 512.f, 64 / 512.f,
                                                64 / 512.f));
        FontHelper::drawNumber(spriteBatcher,1216-64, 64 + 32, world->coin);

        if (index == 2)
            spriteBatcher->drawSprite(64 + 64, 64 + 96, 544 - 64, 200,
                                      TextureRegion((384 + 64) / 512.f, 128 / 512.f, 64 / 512.f,
                                                    64 / 512.f));
        else
            spriteBatcher->drawSprite(64 + 64, 64 + 96, 544 - 64, 200,
                                      TextureRegion(384 / 512.f, 128 / 512.f, 64 / 512.f,
                                                    64 / 512.f));
        spriteBatcher->drawSprite(64 + 64 + 16, 64 + 96 + 16, 64, 64,
                                  TextureRegion(384 / 512.f, 448 / 512.f, 64 / 512.f,
                                                64 / 512.f));
        FontHelper::drawText(spriteBatcher,64 + 64 + 16 + 64 + 16, 64 + 96 + 16, "ATK +2");
        FontHelper::drawText(spriteBatcher,64 + 64 + 16 + 64 + 16, 64 + 96 + 16 + 32 + 16, "25 G");

        if (index == 3)
            spriteBatcher->drawSprite(672, 64 + 96, 544 - 64, 200,
                                      TextureRegion((384 + 64) / 512.f, 128 / 512.f, 64 / 512.f,
                                                    64 / 512.f));
        else
            spriteBatcher->drawSprite(672, 64 + 96, 544 - 64, 200,
                                      TextureRegion(384 / 512.f, 128 / 512.f, 64 / 512.f,
                                                    64 / 512.f));
        spriteBatcher->drawSprite(672 + 16, 64 + 96 + 16, 64, 64,
                                  TextureRegion(448 / 512.f, 448 / 512.f, 64 / 512.f,
                                                64 / 512.f));
        FontHelper::drawText(spriteBatcher,672 + 16 + 64 + 16, 64 + 96 + 16, "DEF +2");
        FontHelper::drawText(spriteBatcher,672 + 16 + 64 + 16, 64 + 96 + 16 + 32 + 16, "25 G");

        if (index == 4)
            spriteBatcher->drawSprite(64 + 64, 64 + 96 + 200 + 64, 544 - 64, 200,
                                      TextureRegion((384 + 64) / 512.f, 128 / 512.f, 64 / 512.f,
                                                    64 / 512.f));
        else
            spriteBatcher->drawSprite(64 + 64, 64 + 96 + 200 + 64, 544 - 64, 200,
                                      TextureRegion(384 / 512.f, 128 / 512.f, 64 / 512.f,
                                                    64 / 512.f));
        spriteBatcher->drawSprite(64 + 64 + 16, 64 + 96 + 200 + 64 + 16, 64, 64,
                                  TextureRegion(256 / 512.f, 448 / 512.f, 64 / 512.f,
                                                64 / 512.f));
        FontHelper::drawText(spriteBatcher,64 + 64 + 16 + 64 + 16, 64 + 96 + 200 + 64 + 16, "Potion (S)");
        FontHelper::drawText(spriteBatcher,64 + 64 + 16 + 64 + 16, 64 + 96 + 200 + 64 + 16 + 32 + 16, "10 G");

        spriteBatcher->endBatch();
    }

    void onReceive(Event what, const void *arg) override {

    }
};


#endif //RPG2D_GAMESTATESHOP_H
