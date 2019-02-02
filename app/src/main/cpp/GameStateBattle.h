//
// Created by Xyy on 2018/12/26.
//

#ifndef RPG2D_GAMESTATEBATTLE_H
#define RPG2D_GAMESTATEBATTLE_H


#include "EventListener.h"
#include "MultiTouchHandler.h"
#include "Battle.h"
#include "Db.h"
#include "FontHelper.h"

class GameStateBattle final : public Screen {

private:

    World *world;

    EventListener *eventListener;

    Battle *battle;

    float timer = 0;

    Monster *monster;

    Property *player;

    Property *monsterProperty;

    int tileSetIndex = -1;

    TileSet *tileSet;

    int state = 0;

    float bgAlpha = 0;

    int prevPlayerHp = 0;

    int prevMonsterHp = 0;

    int curPlayerHp = 0;

    int curMonsterHp = 0;

    int playerHp = 0;

    int monsterHp = 0;

    float damageX = -1;

    float damageScale = 0;

    short skip = -1;

    void drawNumber(SpriteBatcher *spriteBatcher, float x, float y, unsigned int num) {

        if (num == 0)
            spriteBatcher->drawSprite(x - 35, y, 35, 55, TextureRegion(0, 192, 35, 55));
        else {
            unsigned char i = 0;
            while (num != 0) {
                spriteBatcher->drawSprite(x - 40 * i - 35, y, 35, 55,
                                          TextureRegion(35 * (num % 10), 192, 35, 55));
                num = num / 10;
                i++;
            }
        }
    }

    void drawNumber(SpriteBatcher *spriteBatcher, float x, float y, unsigned int num, float timer) {

        if (num == 0) {
            float scale = 1 + (1 - timer);
            spriteBatcher->drawSprite(x - 20 - 20 * scale, y - (20 * scale - 20), 40 * scale,
                                      40 * scale,
                                      TextureRegion(0, 192, 40, 40));
        } else {
            // Count the number of digits
            unsigned short n = 0;
            unsigned int _num = num;
            while (_num != 0) {
                _num = _num / 10;
                n++;
            }
            float dt = 0.4f / (n + 1);
            unsigned short i = 0;
            while (num != 0) {
                if (timer > (n - i - 1) * dt) {
                    float scale = 2 + ((n - i - 1) * dt - timer) / (2 * dt);
                    if (scale < 1)
                        scale = 1;
                    spriteBatcher->drawSprite(x + n * 22 - 40 * (i + 1) - 20 * (scale - 1),
                                              y - (27 * scale), 40 * scale, 54 * scale,
                                              TextureRegion(40 * (num % 10), 576, 40, 54));
                }
                num = num / 10;
                i++;
            }
        }
    }

public:

    GameStateBattle(Game game, World *world, EventListener *eventListener,
                    unsigned int monsterId) :
            Screen(game), world(world), eventListener(eventListener) {
        player = new Property(*(world->player.prop));
        monster = world->db->getMonsterById(monsterId);
        monsterProperty = monster->prop;
        battle = new Battle();
        battle->newBattle(monsterProperty, player);
        playerHp = prevPlayerHp = curPlayerHp = player->hp;
        monsterHp = prevMonsterHp = curMonsterHp = monsterProperty->hp;

        int layerCount = world->map->layerCount;
        for (int i = 0; i < layerCount; i++) {
            MapLayer *mapLayer = world->map->layers[i];
            if (mapLayer->layerType == MapLayer::OBJECT_LAYER) {
                ObjectLayer *objectLayer = (ObjectLayer *) mapLayer;
                // Note: Here assume that the map only has ONE layer containing monsters
                MapObject *object = objectLayer->objects[0];
                if (object->type == "monster") {
                    tileSetIndex = objectLayer->tileSetIndex;
                    break;
                }
            }
        }

        tileSet = world->map->tileSets[tileSetIndex];
    }

    virtual void resume() override {

    }

    virtual void update(float deltaTime) override {

        std::vector<Touch> &touchEvents = game.input->getTouchEvents();

        if (state == 0) {
            timer += deltaTime;
            if (timer <= 0.2) {
                bgAlpha = timer / 0.2f * 0.5f;
            } else {
                unsigned long len = touchEvents.size();
                for (unsigned long i = 0; i < len; i++) {
                    Touch event = touchEvents[i];
                    if (event.action == TOUCH_DOWN) {
                        if (skip == -1) {
                            skip = event.pointer;
                            break;
                        }
                    } else if (event.action == TOUCH_UP) {
                        if (event.pointer == skip) {
                            skip = -2;

                            prevPlayerHp = player->hp;
                            prevMonsterHp = monsterProperty->hp;

                            battle->skip();

                            curPlayerHp = player->hp;
                            curMonsterHp = monsterProperty->hp;
                            if (battle->isPlayerTurn())
                                damageX = 488;
                            else
                                damageX = 792;
                            damageScale = 0;

                            timer = 0.2f;

                            break;
                        }
                    }
                }
                if (timer <= 0.6f) {
                    monsterHp = static_cast<int>(curMonsterHp +
                                                 (prevMonsterHp - curMonsterHp) * (0.6 - timer) *
                                                 2.5);
                    playerHp = static_cast<int>(curPlayerHp +
                                                (prevPlayerHp - curPlayerHp) * (0.6 - timer) * 2.5);
                    damageScale = (timer - 0.2f) * 2.5f;
                } else {
                    timer -= 0.4f;
                    prevPlayerHp = player->hp;
                    prevMonsterHp = monsterProperty->hp;

                    if (!battle->next()) {
                        if (battle->isWin()) {
                            // Win/胜利
                            state = 1;

                            // Gain items
                            world->coin += monster->coin;
                            world->accessExp(monster->exp);

                            world->player.prop->hp = player->hp;
                            world->removeMonster();
                        } else {
                            // Lose.../失败...
                            state = 2;
                        }
                        timer = 0;
                    }
                    curPlayerHp = player->hp;
                    curMonsterHp = monsterProperty->hp;
                    if (battle->isPlayerTurn())
                        damageX = 488;
                    else
                        damageX = 792;
                    damageScale = 0;
                }
            }
        } else if (timer < 0.5) {
            timer += deltaTime;
        } else {
            if (touchEvents.size() > 0) {
                eventListener->onReceive(Event::RUNNING, nullptr);
            }
        }
    }

    virtual void present() override {

        SpriteBatcher *spriteBatcher = game.graphic;

        // Background
        spriteBatcher->beginBatch(0.f, 0.f, 0.f, bgAlpha);
        spriteBatcher->drawSprite(0, 0, 1280, 720, TextureRegion(0, 0, 1, 1));
        spriteBatcher->endBatch();

        if (state == 0) {
            // Monster
            spriteBatcher->beginBatch(Assets::tileSetTextures[tileSetIndex]);
            int index = monster->gid;
            int y2 = (index / tileSet->columns);
            int x1 = (index % tileSet->columns);
            spriteBatcher->drawSprite(368, 240, 240, 240,
                                      TextureRegion(x1 * tileSet->tileWidth,
                                                    y2 * tileSet->tileHeight,
                                                    tileSet->tileWidth, tileSet->tileHeight));
            spriteBatcher->endBatch();

            // Player
            spriteBatcher->beginBatch(Assets::player);
            spriteBatcher->drawSprite(702, 240, 180, 240, TextureRegion(0, 0, 90, 120));
            spriteBatcher->endBatch();

            spriteBatcher->beginBatch(Assets::ui);
            // Arrow
            if (battle->isPlayerTurn())
                spriteBatcher->drawSprite(608, 328, 64, 64, TextureRegion(384, 0, 64, 64));
            else
                spriteBatcher->drawSprite(672, 328, -64, 64, TextureRegion(384, 0, 64, 64));
            // Damage
            if (damageX > 0)
                drawNumber(spriteBatcher, damageX, 360, battle->getDamage(), damageScale);
            // HP
            drawNumber(spriteBatcher, 608, 440, monsterHp);
            drawNumber(spriteBatcher, 912, 440, playerHp);
            spriteBatcher->endBatch();
        } else if (state == 1) {
            // Win
            spriteBatcher->beginBatch(Assets::ui);

            spriteBatcher->drawSprite(448, 232, 384, 128, TextureRegion(0, 512, 192, 64));

            FontHelper::drawText(spriteBatcher, 568, 424, "Coin");
            FontHelper::drawNumber(spriteBatcher, 730, 424, monster->coin);

            FontHelper::drawText(spriteBatcher, 568, 488, "Exp");
            FontHelper::drawNumber(spriteBatcher, 730, 488, monster->exp);

            spriteBatcher->endBatch();
        } else if (state == 2) {
            // Lose...
            spriteBatcher->beginBatch(Assets::ui);
            spriteBatcher->drawSprite(320, 232, 640, 128, TextureRegion(192, 512, 320, 64));
            spriteBatcher->endBatch();
        }
    }

    void onReceive(Event what, const void *arg) override {

    }

    virtual ~GameStateBattle() override {

        delete battle;
        delete player;
    }
};


#endif //RPG2D_GAMESTATEBATTLE_H
