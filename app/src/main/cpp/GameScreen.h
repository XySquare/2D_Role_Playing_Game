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
#include "GameStateDialog.h"
#include "GameStatePending.h"


class GameScreen final : public Screen, public DialogListener {

private:

    static const unsigned char DIALOG_ESCAPE = 1;

    // A file descriptor to be polled by the looper.
    int messageQueue;

    World *world;

    WorldRenderer *worldRenderer;

    Notification *notification;

    Screen *pendingState;

    Screen *gameState;

public:

    GameScreen(Game game, int fd) : Screen(game), messageQueue(fd) {

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
            gameState = new GameStateTransfer(game, this);
        } else if (what == BATTLE) {
            delete gameState;
            gameState = new GameStateBattle(game, world, this,
                                            static_cast<unsigned int>(*((int *) arg)));
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
        } else if (what == SAVE_MAP) {
            SaveDataHelper::saveMap(world->curMap, world->map);
        } else if (what == LOAD_MAP) {
            world->loadMap(game.fileIO);
            SaveDataHelper::loadMap(world->curMap, world->map);
            worldRenderer->resume();
            world->player.state = Player::PLAYER_STATE_DOWN;
        } else if (what == ESCAPE) {
            delete gameState;
            gameState = new GameStateDialog(game, DIALOG_ESCAPE,
                                            "Your current progress will lose. Do you want to escape ?",
                                            "Confirm", "Cancel", this);
        } else if (what == NOTIFICATION) {
            notification->newNotice((const char *) arg);
        } else if (what == WEBVIEW) {
            pendingState = gameState;
            gameState = new GameStatePending(game);
            // send messages from render to UI thread
            write(messageQueue, &arg, sizeof(arg));
        } else if (what == RESUME) {
            if(pendingState != nullptr){
                delete gameState;
                gameState = pendingState;
                pendingState = nullptr;
            }
        } else {
            gameState->onReceive(what, arg);
        }
    }

    void onDialogResult(unsigned char dialogId, Result res) override {

        if (dialogId == DIALOG_ESCAPE) {
            // Escape
            if (res == APPROVE) {
                world->reset();
                SaveDataHelper::clear();
                onReceive(TRANSFER, nullptr);
            } else
                onReceive(RUNNING, nullptr);
        }
    }

    virtual ~GameScreen() override {

        delete world;
        delete worldRenderer;
        delete gameState;
    }
};


#endif //RPG2D_GAMESCREEN_H
