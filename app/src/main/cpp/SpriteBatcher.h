//
// Created by Xyy on 2018/12/18.
//

#ifndef RPG2D_SPRITEBATCHER_H
#define RPG2D_SPRITEBATCHER_H


#include "Texture.h"
#include "Vertex.h"
#include "TextureRegion.h"
#include "Assets.h"

class SpriteBatcher {

private:

    Vertex *vertices;
    GLfloat *verticesBuffer;
    GLuint *indices;
    int bufferIndex;
    int numSprites;

public:

    SpriteBatcher(unsigned int maxSprites):bufferIndex(0),numSprites(0) {

        verticesBuffer = new GLfloat[4 * 4 * maxSprites];
        vertices = new Vertex(true);

        unsigned int len = maxSprites * 6;
        indices = new GLuint[len];
        GLuint j = 0;
        for (unsigned int i = 0; i < len; i += 6, j += 4) {
            indices[i + 0] = (j + 0);
            indices[i + 1] = (j + 1);
            indices[i + 2] = (j + 2);
            indices[i + 3] = (j + 2);
            indices[i + 4] = (j + 3);
            indices[i + 5] = (j + 0);
        }
        vertices->setIndices(indices);
    }

    void beginBatch(Texture *texture) {

        glVertexAttrib4f(GRAPHIC_COLOR_HANDEL, 1.f, 1.f, 1.f, 1.f);
        texture->bind();
        numSprites = 0;
        bufferIndex = 0;
    }

    void beginBatch(float r, float g, float b, float a) {

        glVertexAttrib4f(GRAPHIC_COLOR_HANDEL,r,g,b,a);
        Assets::unitTexture->bind();
        numSprites = 0;
        bufferIndex = 0;
    }

    void endBatch() {

        vertices->setVertices(verticesBuffer);
        vertices->bind();
        vertices->draw(GL_TRIANGLES, numSprites * 6);
    }

    void drawSprite(float x, float y, float width, float height, TextureRegion region) {

        float x1 = x;
        float y1 = y;
        float x2 = x + width;
        float y2 = y + height;

        verticesBuffer[bufferIndex++] = x1;
        verticesBuffer[bufferIndex++] = y1;
        verticesBuffer[bufferIndex++] = region.u1;
        verticesBuffer[bufferIndex++] = region.v1;

        verticesBuffer[bufferIndex++] = x2;
        verticesBuffer[bufferIndex++] = y1;
        verticesBuffer[bufferIndex++] = region.u2;
        verticesBuffer[bufferIndex++] = region.v1;

        verticesBuffer[bufferIndex++] = x2;
        verticesBuffer[bufferIndex++] = y2;
        verticesBuffer[bufferIndex++] = region.u2;
        verticesBuffer[bufferIndex++] = region.v2;

        verticesBuffer[bufferIndex++] = x1;
        verticesBuffer[bufferIndex++] = y2;
        verticesBuffer[bufferIndex++] = region.u1;
        verticesBuffer[bufferIndex++] = region.v2;

        numSprites++;
    }

    virtual ~SpriteBatcher() {

        delete vertices;
        delete[] verticesBuffer;
        delete[] indices;
    }
};


#endif //RPG2D_SPRITEBATCHER_H
