//
// Created by Xyy on 2018/12/16.
//

#ifndef RPG2D_TEXTURE_H
#define RPG2D_TEXTURE_H


#include <jni.h>
#include <GLES2/gl2.h>
#include <string>

class Texture {

private:

    JNIEnv *const env;
    GLuint textureId;

public:

    const std::string fileName;

    Texture(JNIEnv *const env, std::string fileName) : env(env), fileName(fileName) {}

    void load() {

        jclass cls = env->FindClass("xyy/game/rpg2d/AssetsLoader");
        jmethodID mId = env->GetStaticMethodID(cls,"load","(Ljava/lang/String;)I"); // Don't forget ';'
        jstring jstr = env->NewStringUTF(fileName.c_str());
        textureId = (GLuint)(env->CallStaticIntMethod(cls,mId,jstr));
    }

    void bind() {

        glBindTexture(GL_TEXTURE_2D, textureId);
    }

    void dispose() {

        glBindTexture(GL_TEXTURE_2D, textureId);
        glDeleteTextures(1, &textureId);
    }

    ~Texture() {

        dispose();
    }
};


#endif //RPG2D_TEXTURE_H
