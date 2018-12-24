//
// Created by Xyy on 2018/12/16.
//

#ifndef RPG2D_TEXTURE_H
#define RPG2D_TEXTURE_H


#include <jni.h>
#include <GLES2/gl2.h>

class Texture {
private:

    JNIEnv *const env;
    const char* fileName;
    GLuint textureId;

    void load() {

        jclass cls = env->FindClass("xyy/game/rpg2d/AssetsLoader");
        jmethodID mId = env->GetStaticMethodID(cls,"load","(Ljava/lang/String;)I"); // Don't forget ';'
        jstring jstr = env->NewStringUTF(fileName);
        textureId = (GLuint)(env->CallStaticIntMethod(cls,mId,jstr));
    }

public:

    Texture(JNIEnv *const env, const char *fileName) : env(env), fileName(fileName) {

        load();
    }

    void reload() {

        load();
        bind();
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void bind() {

        glBindTexture(GL_TEXTURE_2D, textureId);
    }

    void dispose() {

        glBindTexture(GL_TEXTURE_2D, textureId);
        glDeleteTextures(1, &textureId);
    }
};


#endif //RPG2D_TEXTURE_H
