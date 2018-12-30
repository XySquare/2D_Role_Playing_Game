//
// Created by Xyy on 2018/12/26.
//

#ifndef RPG2D_GAMESTATETRANSFER_H
#define RPG2D_GAMESTATETRANSFER_H


#include "EventListener.h"
#include "MultiTouchHandler.h"
#include "Screen.h"
#include "World.h"
#include "SpriteBatcher.h"

class GameStateTransfer : public Screen {

private:

    World *world;

    SpriteBatcher *spriteBatcher;

    EventListener *eventListener;

    float timer = 0;

public:

    GameStateTransfer(World *world, SpriteBatcher *spriteBatcher, EventListener *eventListener) :
            world(world),spriteBatcher(spriteBatcher),eventListener(eventListener) {}

    virtual void update(float deltaTime, MultiTouchHandler *handler)  {

        handler->getTouchEvents();

        if(world->state == World::NORMAL){
            timer -= deltaTime;
            if(timer <= 0){
                timer = 0;
                eventListener->onEvent(Event::RUNNING,0);
            }
        }else if(world->state == World::PENDING){
            timer += deltaTime;
            if(timer >= 0.5f){
                timer = 0.5f;
                world->resume();
            }
        }
        world->update(deltaTime);
    }

    virtual void present() {

        spriteBatcher->beginBatch(Assets::unitTexture);

        glVertexAttrib4f(GRAPHIC_COLOR_HANDEL,0.f,0.f,0.f,timer * 2);
        spriteBatcher->drawSprite(0, 0, 1280, 720, TextureRegion(0,0,1,1));

        spriteBatcher->endBatch();
    }

    virtual void resume(){

    }

    virtual ~GameStateTransfer() {

    }
};


#endif //RPG2D_GAMESTATETRANSFER_H
