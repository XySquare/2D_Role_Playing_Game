//
// Created by Xyy on 2018/12/16.
//

#ifndef RPG2D_TEXTURE_H
#define RPG2D_TEXTURE_H


#include <jni.h>
#include <GLES2/gl2.h>
#include <string>
#include "Log.h"

class Texture {

private:

    GLuint textureId;

public:

    const std::string fileName;
    const unsigned short width;
    const unsigned short height;

    Texture(const std::string &fileName, const unsigned short width,
            const unsigned short height) : fileName(fileName), width(width),
                                           height(height) {}

    void load(JNIEnv *const env) {

        jclass cls = env->FindClass("xyy/game/rpg2d/TextureLoader");
        jmethodID mId = env->GetStaticMethodID(cls,"load","(Ljava/lang/String;)I"); // Don't forget ';'
        jstring jstr = env->NewStringUTF(fileName.c_str());
        textureId = (GLuint)(env->CallStaticIntMethod(cls,mId,jstr));
        LOGW("Texture","Load %s", fileName.c_str());
    }

    void bind() {

        glBindTexture(GL_TEXTURE_2D, textureId);
    }

    void dispose() {

        glBindTexture(GL_TEXTURE_2D, textureId);
        glDeleteTextures(1, &textureId);
        LOGW("Texture","Delete %s", fileName.c_str());
    }

    ~Texture() {

        dispose();
    }
};


#endif //RPG2D_TEXTURE_H
