//
// Created by Xyy on 2018/12/16.
//

#ifndef RPG2D_SCREEN_H
#define RPG2D_SCREEN_H


#include <jni.h>
#include "MultiTouchHandler.h"

class Screen {
protected:
    JNIEnv *const env;
public:

    Screen(JNIEnv *env) : env(env) {}

    virtual void resume() = 0;

    virtual void update(float deltaTime, MultiTouchHandler *handler) = 0;

    virtual void present(GLuint gvPositionHandle, GLuint gvCoordinateHandle, GLint gvMatrixHandle, glm::mat4 mMVPMatrix) = 0;
};


#endif //RPG2D_SCREEN_H
