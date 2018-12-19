

// OpenGL ES 2.0 code

#include <jni.h>
#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <chrono> //time

#include "glm/mat4x4.hpp"
#include "glm/ext.hpp"
#include "Texture.h"
#include "Vertices.h"
#include "Screen.h"
#include "GameScreen.h"
#include "MultiTouchHandler.h"

#define  LOG_TAG    "libgl2jni"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

long long int nanoTime();

bool isInstantiated = false;

glm::mat4 mMVPMatrix;

GLuint gProgram;
GLuint gvPositionHandle;
GLuint gvCoordinateHandle;
GLint gvMatrixHandle;
//GLint gvTextureHandle;

MultiTouchHandler *gMultiTouchHandler;

Screen *currentScreen;

long long int startTime;

auto gVertexShader =
        "attribute vec4 vPosition;\n" // Per-vertex position information we will pass in.
        "attribute vec2 vCoordinate;\n"
        "uniform mat4 uMVPMatrix;\n"
        "varying vec2 aCoordinate;\n" // This will be passed into the fragment shader.
        "void main() {\n"
        "  gl_Position = uMVPMatrix * vPosition;\n" // gl_Position is a special variable used to store the final position.
        "  aCoordinate=vCoordinate;"
        "}\n";

auto gFragmentShader =
        "precision mediump float;\n" // Set the default precision to medium. We don't need as high of a precision in the fragment shader.
        "uniform sampler2D vTexture;\n"
        "varying vec2 aCoordinate;\n"
        "void main() {\n"
        "  gl_FragColor=texture2D(vTexture,aCoordinate);\n"//"  gl_FragColor = vec4(0.0, 0.0, 0.0, 0.5);\n"
        "}\n";

static void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s\n", name, v);
}

//检查当前程序错误
static void checkGlError(const char* op) {
    for (GLint error = glGetError(); error; error
            = glGetError()) {
        LOGI("after %s() glError (0x%x)\n", op, error);
    }
}

//获取并编译着色器对象
GLuint loadShader(GLenum shaderType, const char* pSource) {
    //创建一个顶点着色器，并将索引交给shader
    GLuint shader = glCreateShader(shaderType);
    if (shader) {
        //将pSource指向的源码关联到shader索引所在的对象，第二个参数表示pSource指向了几个着色器对象
        //最后一个参数表示源码的长度，输入NULL或者0表示以默认的字符串结尾标志'\0'作为源码结束标记
        glShaderSource(shader, 1, &pSource, NULL);
        //编译着色器
        glCompileShader(shader);
        //编译结果
        GLint compiled = 0;
        //获取编译结果，并且将结果赋值给compiled
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                char* buf = (char*) malloc(infoLen);
                if (buf) {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    LOGE("Could not compile shader %d:\n%s\n",
                            shaderType, buf);
                    free(buf);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    return shader;
}

//使用着色器生成着色器程序对象
GLuint createProgram(const char* pVertexSource, const char* pFragmentSource) {
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
    checkGlError("loadShaderGL_VERTEX_SHADER");
    if (!vertexShader) {
        return 0;
    }

    GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
    checkGlError("loadShaderGL_FRAGMENT_SHADER");
    if (!pixelShader) {
        return 0;
    }

    //创建一个程序，并且将索引赋值给program
    GLuint program = glCreateProgram();
    checkGlError("glCreateProgram");
    if (program) {
        //关联着色器并链接程序
        glAttachShader(program, vertexShader);
        checkGlError("glAttachShader_v");
        glAttachShader(program, pixelShader);
        checkGlError("glAttachShader_p");
        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            //保存日志
            GLint bufLength = 0;
            //获取日志信息
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) {
                char* buf = (char*) malloc(bufLength);
                if (buf) {
                    //从日志缓存中取出关于program length个长度的日志，并保存在buf中
                    glGetProgramInfoLog(program, bufLength, NULL, buf);
                    LOGE("Could not link program:\n%s\n", buf);
                    free(buf);
                }
            }
            //删除程序
            glDeleteProgram(program);
            program = 0;
        }
    }
    return program;
}

long long int nanoTime(){
    return std::chrono::duration_cast< std::chrono::nanoseconds >(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}

void instantiate(JNIEnv *env){

    gMultiTouchHandler = new MultiTouchHandler();

    currentScreen = new GameScreen(env);

    // Projection matrix
    mMVPMatrix = glm::ortho( 0.f, 1280.f, 720.f, 0.f, -1.f, 1.f );
}

void onSurfaceCreated(JNIEnv *env){

    if(!isInstantiated){
        instantiate(env);
        isInstantiated = true;
    }

    //uint64_t i;

    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);

    //In a modern core profile, there is no "fixed-function pipeline" (i.e. built-in shader generator). glEnable( GL_TEXTURE_2D) is a directive to the fixed-function pipeline's shader generator that you want to include code to support that texture unit.
    //Delete it. You're writing the shader so you decide directly which texture units you're going to reference.
    // Just glActiveTexture to activate the right texture unit, glBindTexture, then glUniform1i to set the texture unit index on the appropriate sampler uniform.
    //glEnable(GL_TEXTURE_2D);

    //Decide which texture units to reference.
    glActiveTexture(GL_TEXTURE0);

    //apply alpha blending to all triangles we render from this point on
    glEnable(GL_BLEND);

    //Set blending function, specifies how the source and destination color should be combined
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    gProgram = createProgram(gVertexShader, gFragmentShader);
    if (!gProgram) {
        LOGE("Could not create program.");
        return;
    }
    gvPositionHandle = glGetAttribLocation(gProgram, "vPosition");
    gvCoordinateHandle = glGetAttribLocation(gProgram, "vCoordinate");
    gvMatrixHandle = glGetUniformLocation(gProgram, "uMVPMatrix");
    //GLint gvTextureHandle = glGetUniformLocation(gProgram, "vTexture");
    //glUniform1i(gvTextureHandle, 0);
    checkGlError("glGetAttribLocation");

    //使用一个着色器程序(在OpenGL中我们可以持有多个着色器程序)
    //如果希望“冻结”一个着色器程序，则再次调用上述方法，只不过将参数设置为0即可glUseProgram(0)
    glUseProgram(gProgram);
    checkGlError("glUseProgram");

    glEnableVertexAttribArray(gvPositionHandle);
    checkGlError("glEnableVertexAttribArray");

    glEnableVertexAttribArray(gvCoordinateHandle);
    checkGlError("glEnableVertexAttribArray");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    checkGlError("glClearColor");

    glUniformMatrix4fv(gvMatrixHandle,1,GL_FALSE,glm::value_ptr(mMVPMatrix));

    currentScreen->resume();
}

void onSurfaceChanged(int w, int h) {

    glViewport(0, 0, w, h);
    checkGlError("glViewport");

    LOGI("setupGraphics(%d, %d)", w, h);

    startTime = nanoTime();
}

long long int fpsStartTime;
int frames = 0;

void onDrawFrame() {
    long long int curTime = nanoTime();
    float deltaTime = (curTime - startTime) / 1000000000.0f;
    startTime = curTime;

    frames++;
    if(curTime - fpsStartTime >= 1000000000) {
        LOGD("Fps: %d",frames);
        frames = 0;
        fpsStartTime = curTime;
    }

    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    checkGlError("glClear");

    currentScreen->update(deltaTime, gMultiTouchHandler);
    currentScreen->present(gvPositionHandle,gvCoordinateHandle,gvMatrixHandle,mMVPMatrix);
}

extern "C" {
    JNIEXPORT void JNICALL Java_xyy_game_rpg2d_framework_GL2JNILib_onSurfaceCreated(JNIEnv *env, jclass type);
    JNIEXPORT void JNICALL Java_xyy_game_rpg2d_framework_GL2JNILib_onSurfaceChanged(JNIEnv * env, jclass type,  jint width, jint height);
    JNIEXPORT void JNICALL Java_xyy_game_rpg2d_framework_GL2JNILib_onDrawFrame(JNIEnv * env, jclass type);
    JNIEXPORT void JNICALL Java_xyy_game_rpg2d_framework_GL2JNILib_onTouch(JNIEnv *env, jclass type, jint pointer,
                                                                           jshort action, jint x, jint y);
};

JNIEXPORT void JNICALL Java_xyy_game_rpg2d_framework_GL2JNILib_onSurfaceCreated(JNIEnv *env, jclass type) {

    onSurfaceCreated(env);
}

JNIEXPORT void JNICALL Java_xyy_game_rpg2d_framework_GL2JNILib_onSurfaceChanged(JNIEnv *env, jclass type,  jint width, jint height) {

    onSurfaceChanged(width, height);
}

JNIEXPORT void JNICALL Java_xyy_game_rpg2d_framework_GL2JNILib_onDrawFrame(JNIEnv * env, jclass type) {

    onDrawFrame();
}

JNIEXPORT void JNICALL
Java_xyy_game_rpg2d_framework_GL2JNILib_onTouch(JNIEnv *env, jclass type, jint pointer,
                                                jshort action, jint x, jint y) {
    Touch event;
    event.pointer = (int)pointer;
    event.action = (short)action;
    event.x = (int)x;
    event.y = (int)y;
    gMultiTouchHandler->onTouch(event);
}

/*extern "C"
JNIEXPORT void JNICALL
Java_com_android_gl2jni_GL2JNILib_loadImage(JNIEnv *env, jclass type, jobject assetManager) {

    AAssetManager* aAssetManager = AAssetManager_fromJava(env,assetManager);

    // Open file
    AAsset* file = AAssetManager_open(aAssetManager, "img_test.png", AASSET_MODE_BUFFER);
    // Get the file length
    size_t fileLength = AAsset_getLength(file);

    // Allocate memory to read file
    unsigned char* fileContent = new unsigned char[fileLength];

    AAsset_read(file, fileContent, fileLength);

    //Do something
    tex_2d = SOIL_load_OGL_texture_from_memory(fileContent,fileLength,SOIL_LOAD_RGBA,SOIL_CREATE_NEW_ID,SOIL_FLAG_POWER_OF_TWO);

    delete [] fileContent;

}*/

