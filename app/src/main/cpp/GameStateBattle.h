//
// Created by Xyy on 2018/12/26.
//

#ifndef RPG2D_GAMESTATEBATTLE_H
#define RPG2D_GAMESTATEBATTLE_H


#include "EventListener.h"
#include "MultiTouchHandler.h"
#include "Battle.h"
#include "Db.h"

class GameStateBattle : public Screen {

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

    float damageX = 0;

    short skip = -1;

    void drawNumber(SpriteBatcher *spriteBatcher, float x, float y, unsigned int num) {

        if (num == 0)
            spriteBatcher->drawSprite(x - 38, y, 38, 40,
                                      TextureRegion(0, 3 * 64 / 512.f,
                                                    38 / 512.f, 40 / 512.f));
        else {
            unsigned char i = 0;
            while (num != 0) {
                spriteBatcher->drawSprite(x - 38 * i - 38, y, 38, 40,
                                          TextureRegion(38 / 512.f * (num % 10), 3 * 64 / 512.f,
                                                        38 / 512.f, 40 / 512.f));
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
            if (timer <= 0.5) {
                bgAlpha = timer;
                //monsterX = (int) ((490 > monsterTexture.getWidth() ? 490 : monsterTexture.getWidth()) * timer * 2 - monsterTexture.getWidth());
                //playerX = (int) (1280 - 490 * timer * 2);
            } else {
                unsigned long len = touchEvents.size();
                for (int i = 0; i < len; i++) {
                    Touch event = touchEvents[i];
                    if (event.action == Action::TOUCH_DOWN) {
                        if (skip == -1) {
                            skip = event.pointer;
                            break;
                        }
                    } else if (event.action == Action::TOUCH_UP) {
                        if (event.pointer == skip) {
                            skip = -2;

                            prevPlayerHp = player->hp;
                            prevMonsterHp = monsterProperty->hp;

                            battle->skip();

                            curPlayerHp = player->hp;
                            curMonsterHp = monsterProperty->hp;

                            //enemyDamageX = 250;
                            //playerDamageX = 950;
                            //playerDamageY = 300;
                            //playerDamage = String.valueOf(battle.getPlayerDamage());
                            //enemyDamage = String.valueOf(battle.getEnemyDamage());
                            //playerDamageType = battle.getPlayerDamageType();
                            //enemyDamageType = battle.getEnemyDamageType();

                            timer = 0.5f;
                            //damageX = 2000;
                            break;
                        }
                    }
                }
                if (timer <= 0.9) {
                    monsterHp = (int) ((float) curMonsterHp +
                                       (prevMonsterHp - curMonsterHp) * (0.9 - timer) * 2.5);
                    playerHp = (int) ((float) curPlayerHp +
                                      (prevPlayerHp - curPlayerHp) * (0.9 - timer) * 2.5);
                    //playerDamageY = (int) (300 - 100 * (timer - 0.5) * 2.5);
                    //enemyDamageY = (int) (300 - 100 * (timer - 0.5) * 2.5);
                } else {
                    timer -= 0.4;
                    prevPlayerHp = player->hp;
                    prevMonsterHp = monsterProperty->hp;
                    //monsterHpBar = (int) ((float) monster.getHp() / monster.getMaxHp() * 286);
                    //playerHpBar = (int) ((float) player.getHp() / player.getMaxHp() * 286);
                    if (!battle->next()) {
                        if (battle->isWin()) {
                            //胜利
                            state = 1;

                            // Gain items
                            world->coin += monster->coin;
                            world->accessExp(monster->exp);

                            world->player.prop->hp = player->hp;
                            world->removeMonster();
                        } else {
                            //失败...
                            //player.setHp(saveHp);
                            state = 2;
                        }
                        timer = 0;
                    }
                    curPlayerHp = player->hp;
                    curMonsterHp = monsterProperty->hp;
                    if (battle->isPlayerTurn())
                        damageX = 1280 / 2 + 8 - 120 + 20;
                    else
                        damageX = 1280 / 2 + 8 + 120 + 20;
                    //enemyDamageX = 250;
                    //playerDamageX = 950;
                    //playerDamageY = 300;
                    //enemyDamageY = 300;
                    //playerDamage = String.valueOf(battle.getPlayerDamage());
                    //enemyDamage = String.valueOf(battle.getEnemyDamage());
                    //playerDamageType = battle.getPlayerDamageType();
                    //enemyDamageType = battle.getEnemyDamageType();
                }
            }
        } else if (timer < 0.5) {
            timer += deltaTime;
        } else {
            if (touchEvents.size() > 0) {
                eventListener->onReceive(Event::RUNNING, NULL);
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
            spriteBatcher->drawSprite(1280 / 2 - 240 - 8,
                                      720 / 2 - 120, 240,
                                      240,
                                      TextureRegion((float) (x1 * tileSet->tileWidth) /
                                                    tileSet->imageWidth,
                                                    (float) (y2 * tileSet->tileHeight) /
                                                    tileSet->imageHeight,
                                                    (float) tileSet->tileWidth /
                                                    tileSet->imageWidth,
                                                    (float) tileSet->tileHeight /
                                                    tileSet->imageHeight));
            spriteBatcher->endBatch();

            // Player
            spriteBatcher->beginBatch(Assets::player);
            TextureRegion playerRegine(0, 0, 90 / 512.f, 120 / 512.f);
            spriteBatcher->drawSprite(
                    1280 / 2 + 8 + (240 - 180) / 2,
                    720 / 2 - 120, 180, 240,
                    playerRegine);
            spriteBatcher->endBatch();

            spriteBatcher->beginBatch(Assets::ui);
            // Arrow
            if (battle->isPlayerTurn())
                spriteBatcher->drawSprite(1280 / 2 - 32,
                                          720 / 2 - 32, 64,
                                          64,
                                          TextureRegion((float) (6 * 64) / 512, 0, (float) 64 / 512,
                                                        (float) 64 / 512));
            else
                spriteBatcher->drawSprite(1280 / 2 + 32,
                                          720 / 2 - 32, -64,
                                          64,
                                          TextureRegion((float) (6 * 64) / 512, 0, (float) 64 / 512,
                                                        (float) 64 / 512));
            // HP
            drawNumber(spriteBatcher, damageX, 720 / 2, battle->getDamage());
            drawNumber(spriteBatcher, 1280 / 2 - 8, 720 / 2 + 120 - 40, monsterHp);
            drawNumber(spriteBatcher, 1280 / 2 + 8 + 240, 720 / 2 + 120 - 40, playerHp);
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
