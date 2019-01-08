//
// Created by Xyy on 2018/12/26.
//

#ifndef RPG2D_GAMESTATETRANSFER_H
#define RPG2D_GAMESTATETRANSFER_H


#include "Screen.h"
#include "World.h"
#include "SpriteBatcher.h"
#include "SaveDataHelper.h"

class GameStateTransfer final : public Screen {

private:

    World *world;

    SpriteBatcher *spriteBatcher;

    EventListener *eventListener;

    unsigned char state = 0;

    float timer = 0;

public:

    GameStateTransfer(Game game, World *world, SpriteBatcher *spriteBatcher, EventListener *eventListener) :
            Screen(game), world(world), spriteBatcher(spriteBatcher), eventListener(eventListener) {}

    virtual void update(float deltaTime) override {

        game.input->getTouchEvents();

        if(state == 0){
            timer += deltaTime;
            if(timer >= 0.5f){
                timer = 0.5f;
                state = 1;
                SaveDataHelper::saveMap(world->curMap, world->map);
                world->loading(game.fileIO);
                SaveDataHelper::loadMap(world->curMap, world->map);
                state = 2;
            }
        }
        else if(state == 2){
            timer -= deltaTime;
            if(timer <= 0){
                timer = 0;
                eventListener->onEvent(Event::RUNNING,0);
            }
        }
        //world->update(deltaTime);
    }

    virtual void present() override {

        spriteBatcher->beginBatch(Assets::unitTexture);

        glVertexAttrib4f(GRAPHIC_COLOR_HANDEL,0.f,0.f,0.f,timer * 2);
        spriteBatcher->drawSprite(0, 0, 1280, 720, TextureRegion(0,0,1,1));

        spriteBatcher->endBatch();
    }

    virtual void resume() override {

    }

    void onEvent(int what, int prop) override {

    }

    virtual ~GameStateTransfer() {

    }
};


#endif //RPG2D_GAMESTATETRANSFER_H
