//
// Created by Xyy on 2018/12/26.
//

#ifndef RPG2D_GAMESTATERUNNING_H
#define RPG2D_GAMESTATERUNNING_H


#include <math.h>

#include "Screen.h"
#include "World.h"
#include "SpriteBatcher.h"

class GameStateRunning final: public Screen {

private:

    World *world;

    SpriteBatcher *spriteBatcher;

    EventListener *eventListener;

    float controllerX = 150;

    float controllerY = 570;

    short controllerPointer = -1;

    Vector v;

    bool updateController(Touch event, Vector &vector) {

        const int LCtrlX = 150;
        const int LCtrlY = 570;
        const int maxR = 100;
        const int minR = 25;
        int dx = event.x - LCtrlX;
        int dy = event.y - LCtrlY;
        float length = (float) sqrt(dx * dx + dy * dy);
        if (event.action == Action::TOUCH_DOWN) {
            if (length < maxR && controllerPointer == -1) {
                controllerPointer = event.pointer;
                controllerX = event.x;
                controllerY = event.y;
                if (length > minR) {
                    vector.x = (float) dx / length;
                    vector.y = (float) dy / length;
                }
                return true;
            }
        } else if (event.action == Action::TOUCH_DRAGGED) {
            if (event.pointer == controllerPointer) {
                if (length > minR) {
                    vector.x = (float) dx / length;
                    vector.y = (float) dy / length;
                } else {
                    vector.x = 0;
                    vector.y = 0;
                }
                if (length < maxR) {
                    controllerX = event.x;
                    controllerY = event.y;
                } else {
                    controllerX = LCtrlX + vector.x * maxR;
                    controllerY = LCtrlY + vector.y * maxR;
                }
                return true;
            }
        } else if (event.action == Action::TOUCH_UP) {
            if (event.pointer == controllerPointer) {
                controllerPointer = -1;
                vector.x = 0;
                vector.y = 0;
                controllerX = 150;
                controllerY = 570;
                return true;
            }
        }
        return false;
    }

public:

    GameStateRunning(World *world, SpriteBatcher *spriteBatcher, EventListener *eventListener) :
    world(world),spriteBatcher(spriteBatcher),eventListener(eventListener),v(0,0) {}

    virtual void update(float deltaTime, MultiTouchHandler *handler)  {

        std::vector<Touch> touchEvents = handler->getTouchEvents();

        for (Touch e : touchEvents) {
            if (updateController(e, v))
                world->setPlayerVelocity(v);
        }

        world->update(deltaTime);
    }

    virtual void present() {

        spriteBatcher->beginBatch(Assets::ui);

        spriteBatcher->drawSprite(54, 474, 192, 192, TextureRegion(0, 0, (float) 3 * 64 / 512, (float) 3 * 64 / 512));
        spriteBatcher->drawSprite(controllerX - 80, controllerY - 80, 160, 160, TextureRegion((float) (3 * 64) / 512, 0, (float) 2.5 * 64 / 512, (float) 2.5 * 64 / 512));

        spriteBatcher->endBatch();
    }

    virtual void resume(){

    }

    virtual ~GameStateRunning() {

    }
};


#endif //RPG2D_GAMESTATERUNNING_H
