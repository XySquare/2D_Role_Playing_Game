/**
 * A 1x1 texture with RGBA(255,255,255,255)
 *
 * Created by Xyy on 2018/12/25.
 */


#ifndef RPG2D_UNITTEXTURE_H
#define RPG2D_UNITTEXTURE_H


#include <GLES2/gl2.h>

class UnitTexture {

private:

    GLuint textureId;

public:

    void load() {

        glGenTextures(1, &textureId);

        GLubyte data[] = { 255, 255, 255, 255 };

        glBindTexture(GL_TEXTURE_2D, textureId);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }

    void bind() {

        glBindTexture(GL_TEXTURE_2D, textureId);
    }

    void dispose() {

        glBindTexture(GL_TEXTURE_2D, textureId);
        glDeleteTextures(1, &textureId);
    }
};


#endif //RPG2D_UNITTEXTURE_H
