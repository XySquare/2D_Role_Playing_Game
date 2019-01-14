//
// Created by Xyy on 2018/12/16.
//

#ifndef RPG2D_GAMESCREEN_H
#define RPG2D_GAMESCREEN_H


#include "Screen.h"
#include "Vector2.h"
#include "World.h"
#include "WorldRenderer.h"
#include "EventListener.h"
#include "GameStateBattle.h"
#include "GameStateTransfer.h"
#include "GameStateRunning.h"
#include "GameStatePause.h"
#include "GameStateShop.h"
#include "GameStateMonsterInfo.h"
#include "GameStateTitle.h"
#include "Notification.h"

class GameScreen final : public Screen {

private:

    World *world;

    WorldRenderer *worldRenderer;

    Notification *notification;

    Screen *gameState;

public:

    GameScreen(Game game) : Screen(game) {

        world = new World(game.fileIO, this);

        worldRenderer = new WorldRenderer(world, game.graphic);

        gameState = new GameStateTitle(game, world, this);

        notification = new Notification();
    }

    void resume() override {

        worldRenderer->resume();
    }

    void update(float deltaTime) override {

        gameState->update(deltaTime);

        notification->update(deltaTime);
    }

    void present() override {

        worldRenderer->render();

        gameState->present();

        notification->present(game.graphic);
    }

    virtual void onReceive(Event what, const void *arg) override {

        world->setPlayerVelocity(Vector2(0, 0));

        if (what == RUNNING) {
            delete gameState;
            gameState = new GameStateRunning(game, world, this);
        } else if (what == TRANSFER) {
            delete gameState;
            gameState = new GameStateTransfer(game, world, this);
        } else if (what == BATTLE) {
            delete gameState;
            gameState = new GameStateBattle(game, world, this, *((int *) arg));
        } else if (what == SHOP) {
            delete gameState;
            gameState = new GameStateShop(game, world, this);
        } else if (what == PAUSE) {
            delete gameState;
            gameState = new GameStatePause(game, world, this);
        } else if (what == MONSTER_INFO) {
            delete gameState;
            Vector2 *v = (Vector2 *) arg;
            gameState = new GameStateMonsterInfo(game, world, this,
                                                 static_cast<unsigned int>(v->x), v->y < 640);
        } else if (what == ITEM) {
            // ... DO NOT delete gameState when encounter an item
            int itemId = *((int *) arg);
            // Keys
            if (itemId >= 1 && itemId <= 3) {
                world->bag->key[itemId - 1]++;
            }
            // Potion (S)
            else if (itemId == 4) {
                world->bag->potion_s++;
            }
            // Potion (L)
            else if (itemId == 5) {
                world->bag->potion_l++;
            }
        } else if (what == LOAD_MAP) {
            world->loadMap(game.fileIO);
            SaveDataHelper::loadMap(world->curMap, world->map);
            worldRenderer->resume();
        } else if (what == ESCAPE) {
            delete world;
            world = new World(game.fileIO, this);
            SaveDataHelper::clear();
            worldRenderer->resume();
        } else if (what == NOTIFICATION) {
            notification->newNotice((const char *) arg);
        } else {
            gameState->onReceive(what, arg);
        }
    }

    virtual ~GameScreen() override {

        delete world;
        delete worldRenderer;
        delete gameState;
    }
};


#endif //RPG2D_GAMESCREEN_H
