//
// Created by Xyy on 2018/12/16.
//

#ifndef RPG2D_VERTICES_H
#define RPG2D_VERTICES_H


#include <GLES2/gl2.h>

#define  LOGI_Vertices(...)  __android_log_print(ANDROID_LOG_INFO,"Vertices",__VA_ARGS__)

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

    void bind(GLuint positionHandle, GLuint coordinateHandle) {

        glVertexAttribPointer(positionHandle, 2, GL_FLOAT, GL_FALSE, stride, vertices);
        //LOGI_Vertices("positionHandle %f %f",vertices[0],vertices[1]);

        if(hasTexCoords) {
            glVertexAttribPointer(coordinateHandle, 2, GL_FLOAT, GL_FALSE, stride, vertices + 2);
            //LOGI_Vertices("positionHandle %f %f",vertices[2],vertices[3]);
        }
    }

    void draw(GLenum primitiveType, int numVertices) {

        if(indices)
            glDrawElements(primitiveType, numVertices, GL_UNSIGNED_INT, indices);
        else
            glDrawArrays(primitiveType, 0, numVertices);
    }

    //Destructor
    ~Vertex()
    {
        if(vertices)
            delete vertices;
        if(indices)
            delete indices;
    }
};


#endif //RPG2D_VERTICES_H
