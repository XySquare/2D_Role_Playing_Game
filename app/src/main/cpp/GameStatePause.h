/**
 * One of the states of GameScreen. Active when player pause the game.
 *
 * Created by Xyy on 2018/12/30.
 */

#ifndef RPG2D_GAMESTATEPAUSE_H
#define RPG2D_GAMESTATEPAUSE_H


#include "Screen.h"
#include "World.h"
#include "SaveDataHelper.h"
#include "ListView.h"

class GameStatePause final : public Screen {

private:

    World *world;

    EventListener *eventListener;

    signed char pointer = -1;

    unsigned char index = 0;

    ListView *list;

    float timer = 0;

    bool inBound(Touch e, short x, short y, unsigned short width, unsigned short height) {
        return e.x > x && e.x < x + width && e.y > y && e.y < y + height;
    }

public:

    GameStatePause(Game game, World *world, EventListener *eventListener) :
            Screen(game), world(world), eventListener(eventListener) {

        list = new ListView(64, 512, 96, 8, world->visitedList, world);
    }

    void resume() override {

    }

    void update(float deltaTime) override {

        if(timer < 0.5f){
            timer += deltaTime*5;
            if(timer > 0.5f){
                timer = 0.5f;
            }
        }else{
            timer = 0.5f;
        }

        list->update(deltaTime);

        std::vector<Touch> &touchEvents = game.input->getTouchEvents();

        for (Touch e : touchEvents) {
            list->eventHandaler(e);
            if (e.action == Action::TOUCH_DOWN) {
                if (pointer == -1) {
                    pointer = e.pointer;
                    if (e.x > 1216 && e.y < 64) {
                        index = 1;
                    } else if (inBound(e, 814, 576, 192, 96)) {
                        index = 2;
                    } else if (inBound(e, 1024, 576, 192, 96)) {
                        index = 3;
                    } else if (inBound(e, 1120, 448, 96, 96)) {
                        index = 4;
                    } else if (inBound(e, 1008, 448, 96, 96)) {
                        index = 5;
                    } else if (inBound(e, 604, 576, 192, 96)) {
                        index = 6;
                    } else
                        index = 0;
                }
            } else if (e.action == Action::TOUCH_UP) {
                if (pointer == e.pointer) {
                    pointer = -1;
                    if (e.x > 1216 && e.y < 64 && index == 1)
                        eventListener->onReceive(Event::RUNNING, nullptr);
                    else if (inBound(e, 814, 576, 192, 96) && index == 2) {
                        // Save
                        if (SaveDataHelper::save(game.fileIO, world)) {
                            eventListener->onReceive(NOTIFICATION, "Saved Successfully");
                            eventListener->onReceive(RUNNING, nullptr);
                        } else {
                            //TODO: Save failed
                        }
                    } else if (inBound(e, 1024, 576, 192, 96) && index == 3) {
                        // Load
                        if (SaveDataHelper::load(game.fileIO, world)) {
                            //eventListener->onReceive(LOAD_MAP, NULL);
                            eventListener->onReceive(NOTIFICATION, "Loaded Successfully");
                            eventListener->onReceive(TRANSFER, nullptr);
                        } else {
                            eventListener->onReceive(NOTIFICATION, "No Data");
                        }
                    } else if (inBound(e, 1120, 448, 96, 96) && index == 4) {
                        // Potion (S)
                        if (world->bag->potion_s > 0 &&
                            world->player.prop->hp < world->player.prop->maxHp) {
                            world->bag->potion_s--;
                            world->player.prop->hp += 20;
                            if (world->player.prop->hp > world->player.prop->maxHp) {
                                world->player.prop->hp = world->player.prop->maxHp;
                            }
                        }
                    } else if (inBound(e, 1008, 448, 96, 96) && index == 5) {
                        // Potion (L)
                        if (world->bag->potion_l > 0 &&
                            world->player.prop->hp < world->player.prop->maxHp) {
                            world->bag->potion_l--;
                            world->player.prop->hp += 20;
                            if (world->player.prop->hp > world->player.prop->maxHp) {
                                world->player.prop->hp = world->player.prop->maxHp;
                            }
                        }
                    } else if (inBound(e, 604, 576, 192, 96) && index == 6) {
                        // Escape
                        eventListener->onReceive(ESCAPE, nullptr);
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
        spriteBatcher->beginBatch(0.f, 0.f, 0.f, timer);
        spriteBatcher->drawSprite(0, 0, 1280, 720, TextureRegion(0, 0, 1, 1));
        spriteBatcher->endBatch();

        spriteBatcher->beginBatch(Assets::ui);

        list->present(spriteBatcher);

        // Close
        spriteBatcher->drawSprite(1216, 0, 64, 64,
                                  TextureRegion(448, 64, 64, 64));

        FontHelper::drawText(spriteBatcher, 910, 64, "LV");
        FontHelper::drawNumber(spriteBatcher, 1018, 64, world->player.prop->lv);

        FontHelper::drawText(spriteBatcher, 1036, 64, "NEXT");
        FontHelper::drawNumber(spriteBatcher, 1216, 64, world->getExp());

        FontHelper::drawText(spriteBatcher, 910, 128, "HP");
        FontHelper::drawNumber(spriteBatcher, 1108, 128, world->player.prop->hp);
        FontHelper::drawText(spriteBatcher, 1108, 128, "/");
        FontHelper::drawNumber(spriteBatcher, 1216, 128, world->player.prop->maxHp);

        FontHelper::drawText(spriteBatcher, 910, 192, "ATK");
        FontHelper::drawNumber(spriteBatcher, 1216, 192, world->player.prop->atk);
        FontHelper::drawText(spriteBatcher, 910, 256, "DEF");
        FontHelper::drawNumber(spriteBatcher, 1216, 256, world->player.prop->def);
        FontHelper::drawText(spriteBatcher, 910, 320, "AGI");
        FontHelper::drawNumber(spriteBatcher, 1216, 320, world->player.prop->agi);

        // Coin
        spriteBatcher->drawSprite(910, 376, 48, 48, TextureRegion(0, 448, 64, 64));
        FontHelper::drawNumber(spriteBatcher, 1216, 384, world->coin);

        // Keys
        spriteBatcher->drawSprite(896, 448, 96, 96, TextureRegion(384, 64, 64, 64));
        spriteBatcher->drawSprite(912, 464, 64, 64, TextureRegion(64, 448, 64, 64));
        FontHelper::drawNumber(spriteBatcher, 988, 512, world->bag->key[0]);

        spriteBatcher->drawSprite(784, 448, 96, 96, TextureRegion(384, 64, 64, 64));
        spriteBatcher->drawSprite(800, 464, 64, 64, TextureRegion(128, 448, 64, 64));
        FontHelper::drawNumber(spriteBatcher, 876, 512, world->bag->key[1]);

        spriteBatcher->drawSprite(672, 448, 96, 96, TextureRegion(384, 64, 64, 64));
        spriteBatcher->drawSprite(688, 464, 64, 64, TextureRegion(192, 448, 64, 64));
        FontHelper::drawNumber(spriteBatcher, 764, 512, world->bag->key[2]);

        // Potion (S)
        spriteBatcher->drawSprite(1120, 448, 96, 96,
                                  TextureRegion((index == 4 ? 448 : 384), 128, 64, 64));
        spriteBatcher->drawSprite(1136, 464, 64, 64, TextureRegion(256, 448, 64, 64));
        FontHelper::drawNumber(spriteBatcher, 1212, 512, world->bag->potion_s);

        // Potion (L)
        spriteBatcher->drawSprite(1008, 448, 96, 96,
                                  TextureRegion((index == 5 ? 448 : 384), 128, 64, 64));
        spriteBatcher->drawSprite(1024, 464, 64, 64,
                                  TextureRegion(320, 448, 64, 64));
        FontHelper::drawNumber(spriteBatcher, 1100, 512, world->bag->potion_l);

        // LOAD
        spriteBatcher->drawSprite(1024, 576, 192, 96,
                                  TextureRegion((index == 3 ? 448 : 384), 128, 64, 64));
        FontHelper::drawText(spriteBatcher, 1104, 612, "LOAD");

        // SAVE
        spriteBatcher->drawSprite(814, 576, 192, 96,
                                  TextureRegion((index == 2 ? 448 : 384), 128, 64, 64));
        FontHelper::drawText(spriteBatcher, 894, 612, "SAVE");

        // Escape
        spriteBatcher->drawSprite(604, 576, 192, 96,
                                  TextureRegion((index == 6 ? 448 : 384), 128, 64, 64));
        FontHelper::drawText(spriteBatcher, 684, 612, "ESC");

        spriteBatcher->endBatch();
    }

    void onReceive(Event what, const void *arg) override {

    }

    ~GameStatePause() override {

        delete list;
    }
};


#endif //RPG2D_GAMESTATEPAUSE_H
