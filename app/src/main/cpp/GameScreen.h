//
// Created by Xyy on 2018/12/16.
//

#ifndef RPG2D_GAMESCREEN_H
#define RPG2D_GAMESCREEN_H

#include <math.h>

#include "Screen.h"
#include "Texture.h"
#include "Vertex.h"
#include "MultiTouchHandler.h"
#include "SpriteBatcher.h"
#include "Vector.h"
#include "World.h"
#include "WorldRenderer.h"

#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,"GameScreen",__VA_ARGS__)

class GameScreen: public Screen {
private:

    World world;
    WorldRenderer *worldRenderer;
    Texture *ui;
    TextureRegion *pa, *pb;
    SpriteBatcher *spriteBatcher;

    float controllerX = 150;
    float controllerY = 570;
    short controllerPointer = -1;

    bool updateController(Touch event, Vector &vector){

        const int LCtrlX = 150;
        const int LCtrlY = 570;
        const int maxR = 100;
        const int minR = 25;
        int dx = event.x - LCtrlX;
        int dy = event.y - LCtrlY;
        float length = (float) sqrt(dx * dx + dy * dy);
        if (event.action == TOUCH_DOWN) {
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
        } else if (event.action == TOUCH_DRAGGED) {
            if (event.pointer == controllerPointer) {
                if(length > minR){
                    vector.x = (float) dx / length;
                    vector.y = (float) dy / length;
                }else{
                    vector.x = 0;
                    vector.y = 0;
                }
                if(length < maxR){
                    controllerX = event.x;
                    controllerY = event.y;
                }else{
                    controllerX = LCtrlX + vector.x * maxR;
                    controllerY = LCtrlY + vector.y * maxR;
                }
                return true;
            }
        } else if (event.action == TOUCH_UP) {
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

    Vector v;

    GameScreen(JNIEnv *env) : Screen(env),v(0.f,0.f) {

        spriteBatcher = new SpriteBatcher(273*2+1);
        ui = new Texture(env, "ui.png");
        pa = new TextureRegion(0,0,(float)3*64/512,(float)3*64/512);
        pb = new TextureRegion((float)(3 * 64)/512,0,(float)2.5*64/512,(float)2.5*64/512);
        worldRenderer = new WorldRenderer(env, &world, spriteBatcher);
    }

    void resume() override {

        worldRenderer->reload();
        ui->reload();
    }

    void update(float deltaTime, MultiTouchHandler* handler) override {

        std::vector<Touch> touchEvents = handler->getTouchEvents();

        for(Touch e : touchEvents) {
            if(updateController(e, v))
                world.setPlayerVelocity(v);
        }

        world.update(deltaTime);
    }

    void present(GLuint gvPositionHandle, GLuint gvCoordinateHandle) override {

        worldRenderer->render(gvPositionHandle, gvCoordinateHandle);

        //UI
        spriteBatcher->beginBatch(ui);

        //Draw Pad
        spriteBatcher->drawSprite(54,474,192,192,*pa);
        spriteBatcher->drawSprite(controllerX-80,controllerY-80,160,160,*pb);

        spriteBatcher->endBatch(gvPositionHandle, gvCoordinateHandle);
    }
};

//glUniformMatrix4fv(gvMatrixHandle,1,GL_FALSE,glm::value_ptr(mMVPMatrix));

#endif //RPG2D_GAMESCREEN_H
