//
// Created by Xyy on 2018/12/18.
//

#ifndef RPG2D_SPRITEBATCHER_H
#define RPG2D_SPRITEBATCHER_H


#include "Texture.h"
#include "Vertices.h"
#include "TextureRegion.h"

#define  LOGI_SpriteBatcher(...)  __android_log_print(ANDROID_LOG_INFO,"SpriteBatcher",__VA_ARGS__)

class SpriteBatcher {
private:
    Vertices* vertices;
    GLfloat* verticesBuffer;
    GLuint *indices;
    int bufferIndex;
    int numSprites;
public:
    SpriteBatcher(int maxSprites) {
        verticesBuffer = new GLfloat[4*4*maxSprites];
        vertices = new Vertices(true);

        bufferIndex = 0;
        numSprites = 0;

        int len = maxSprites * 6;
        indices = new GLuint[len];
        GLuint j = 0;
        for (int i = 0; i < len; i += 6, j += 4) {
            indices[i + 0] = (j + 0);
            indices[i + 1] = (j + 1);
            indices[i + 2] = (j + 2);
            indices[i + 3] = (j + 2);
            indices[i + 4] = (j + 3);
            indices[i + 5] = (j + 0);
        }
        vertices->setIndices(indices);
    }

    void beginBatch(Texture* texture) {

        texture->bind();
        numSprites = 0;
        bufferIndex = 0;
    }

    void endBatch(GLuint positionHandle, GLuint coordinateHandle) {
        //LOGI_SpriteBatcher("verticesBuffer %f,%f",verticesBuffer[0],verticesBuffer[1]);
        //LOGI_SpriteBatcher("indices %d,%d",indices[0],indices[1]);
        vertices->setVertices(verticesBuffer);
        vertices->bind(positionHandle, coordinateHandle);
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
if(vertices)
    delete vertices;
if(verticesBuffer)
    delete verticesBuffer;
    }
};


#endif //RPG2D_SPRITEBATCHER_H
