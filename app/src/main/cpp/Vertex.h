//
// Created by Xyy on 2018/12/16.
//

#ifndef RPG2D_VERTICES_H
#define RPG2D_VERTICES_H


#include <GLES2/gl2.h>
#include "Graphic.h"

class Vertex {

private:

    const bool hasTexCoords;
    const int stride;
    GLfloat *vertices;
    GLuint *indices;

public:

    Vertex(const bool hasTexCoords) : hasTexCoords(hasTexCoords),
                                        stride((2 + (hasTexCoords?2:0)) * sizeof(GLfloat)){
        indices = NULL;
    }

    void setVertices(GLfloat vertices[]) {
        Vertex::vertices = vertices;
    }

    void setIndices(GLuint indices[]) {
        Vertex::indices = indices;
    }

    void bind() {

        glVertexAttribPointer(GRAPHIC_VERTEX_HANDLE, 2, GL_FLOAT, GL_FALSE, stride, vertices);

        if(hasTexCoords) {
            glVertexAttribPointer(GRAPHIC_TEXTURE_HANDEL, 2, GL_FLOAT, GL_FALSE, stride, vertices + 2);
        }
    }

    void draw(GLenum primitiveType, int numVertices) {

        if(indices)
            glDrawElements(primitiveType, numVertices, GL_UNSIGNED_INT, indices);
        else
            glDrawArrays(primitiveType, 0, numVertices);
    }

    ~Vertex() {

    }
};


#endif //RPG2D_VERTICES_H
