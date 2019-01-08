//
// Created by Xyy on 2018/12/16.
//

#ifndef RPG2D_GAMESCREEN_H
#define RPG2D_GAMESCREEN_H


#include "Screen.h"
#include "Texture.h"
#include "Vertex.h"
#include "SpriteBatcher.h"
#include "Vector.h"
#include "World.h"
#include "WorldRenderer.h"
#include "EventListener.h"
#include "GameStateBattle.h"
#include "GameStateTransfer.h"
#include "GameStateRunning.h"
#include "GameStatePause.h"
#include "GameStateShop.h"

class GameScreen : public Screen {

private:

    World *world;

    WorldRenderer *worldRenderer;

    SpriteBatcher *spriteBatcher;

public:

    Screen *gameState;

    GameScreen(Game game) : Screen(game) {

        world = new World(this);

        spriteBatcher = new SpriteBatcher(1024);

        worldRenderer = new WorldRenderer(world, spriteBatcher);

        gameState = new GameStateRunning(game, world, spriteBatcher, this);

        world->loading(game.fileIO);
    }

    void resume() override {

    }

    void update(float deltaTime) override {

        gameState->update(deltaTime);
    }

    void present() override {

        worldRenderer->render();

        gameState->present();
    }

    virtual void onEvent(int what, int prop) override {
        LOGI("onEvent(%d,%d)",what,prop);
        world->setPlayerVelocity(Vector(0,0));

        if(what == Event::RUNNING){
            delete gameState;
            gameState = new GameStateRunning(game, world, spriteBatcher, this);
        }else if(what == Event::TRANSFER){
            delete gameState;
            gameState = new GameStateTransfer(game, world, spriteBatcher, this);
        }else if(what == Event::BATTLE){
            delete gameState;
            gameState = new GameStateBattle(game, world, spriteBatcher, this, prop);
        }else if(what == Event::PAUSE){
            delete gameState;
            gameState = new GameStatePause(game, world, spriteBatcher, this);
        }else if(what == Event::ITEM){
            // ... DO NOT delete gameState when encounter an item
            // ItemID
            if(prop == 1){
                //world.player.prop->agi += 10;
                world->bag->key[0]++;
            }
        }else if(what == Event::SHOP){
            delete gameState;
            gameState = new GameStateShop(game, world, spriteBatcher, this);
        }else{
            gameState->onEvent(what, prop);
        }
    }

    virtual ~GameScreen() override {

        delete world;
        delete spriteBatcher;
        delete worldRenderer;
        delete gameState;
    }
};


#endif //RPG2D_GAMESCREEN_H
