//
// Created by Xyy on 2018/12/16.
//

#ifndef RPG2D_GAMESCREEN_H
#define RPG2D_GAMESCREEN_H


#include "Screen.h"
#include "Texture.h"
#include "Vertex.h"
#include "MultiTouchHandler.h"
#include "SpriteBatcher.h"
#include "Vector.h"
#include "World.h"
#include "WorldRenderer.h"
#include "EventListener.h"
#include "GameStateBattle.h"
#include "GameStateTransfer.h"
#include "GameStateRunning.h"

class GameScreen : public Screen, public EventListener {

private:

    World world;

    WorldRenderer *worldRenderer;

    SpriteBatcher *spriteBatcher;

public:

    Screen *gameState;

    GameScreen() : world(World(*this)) {

        spriteBatcher = new SpriteBatcher(1024/*273*2+1*/);

        worldRenderer = new WorldRenderer(&world, spriteBatcher);

        gameState = new GameStateRunning(&world,spriteBatcher,this);
    }

    void resume() override {

    }

    void update(float deltaTime, MultiTouchHandler *handler) override {

        gameState->update(deltaTime, handler);
    }

    void present() override {

        worldRenderer->render();

        gameState->present();
    }

    virtual void onEvent(int what, int prop) override {
        LOGI("onEvent(%d,%d)",what,prop);
        world.setPlayerVelocity(Vector(0,0));

        if(what == Event::RUNNING){
            if(gameState)
                delete gameState;
            gameState = new GameStateRunning(&world,spriteBatcher,this);
        }else if(what == Event::TRANSFER){
            if(gameState)
                delete gameState;
            gameState = new GameStateTransfer(&world,spriteBatcher,this);
        }else if(what == Event::BATTLE){
            if(gameState)
                delete gameState;
            gameState = new GameStateBattle(&world,spriteBatcher,this,prop);
        }else if(what == Event::ITEM){
            // ... DO NOT delete gameState when encounter an item
            // ItemID
            if(prop == 1){
                world.player.prop->agi += 10;
            }
        }
    }

    virtual ~GameScreen() override {

        delete spriteBatcher;
        delete worldRenderer;
        delete gameState;
    }
};


#endif //RPG2D_GAMESCREEN_H
