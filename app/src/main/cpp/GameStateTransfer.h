/**
 * One of the states of GameScreen. Active when player transfers between different maps.
 * The screen will get dark, new map will be loaded, and screen will become bright again.
 *
 * Created by Xyy on 2018/12/26.
 */
#ifndef RPG2D_GAMESTATETRANSFER_H
#define RPG2D_GAMESTATETRANSFER_H


#include "Screen.h"
#include "World.h"
#include "SaveDataHelper.h"

class GameStateTransfer final : public Screen {

private:

    World *world;

    EventListener *eventListener;

    // TODO: UNKONWN ERROR - when _state = 0, in some case it will be 255
    unsigned char _state = 1;

    float timer = 0;

public:

    GameStateTransfer(Game game, World *world, EventListener *eventListener) :
            Screen(game), world(world), eventListener(eventListener),_state(1) {}

    virtual void resume() override {

    }

    virtual void update(float deltaTime) override {

        game.input->getTouchEvents();

        if(_state == 1){
            timer += deltaTime;
            if(timer >= 0.5f){
                timer = 0.5f;
                SaveDataHelper::saveMap(world->curMap, world->map);
                eventListener->onReceive(LOAD_MAP, NULL);
                _state = 2;
            }
        }
        else if(_state == 2){
            timer -= deltaTime;
            if(timer <= 0){
                timer = 0;
                eventListener->onReceive(Event::RUNNING, NULL);
            }
        }
    }

    virtual void present() override {

        SpriteBatcher *spriteBatcher = game.graphic;

        spriteBatcher->beginBatch(0.f,0.f,0.f,timer * 2);

        spriteBatcher->drawSprite(0, 0, 1280, 720, TextureRegion(0,0,1,1));

        spriteBatcher->endBatch();
    }

    void onReceive(Event what, const void *arg) override {

    }

    virtual ~GameStateTransfer() {

    }
};


#endif //RPG2D_GAMESTATETRANSFER_H
