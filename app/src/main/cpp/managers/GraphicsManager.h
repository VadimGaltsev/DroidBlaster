//
// Graphic manager in app
//

#ifndef ANDROIDNATIVEBLUSTER_GRAPHICSMANAGER_H
#define ANDROIDNATIVEBLUSTER_GRAPHICSMANAGER_H

#include "../Types.h"
#include "../../../../../../../Library/Android/sdk/ndk-bundle/sources/android/native_app_glue/android_native_app_glue.h"
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <png.h>
#include "../helpers/Resource.h"
namespace support {
    void callBackReadPng(png_structp structp, png_bytep data, png_size_t size);
}


static const char * VERTEX_SHADER = {
                "attribute vec2 aPosition; \n"
                "attribute vec2 aTexture; \n"
                "varying vec2 vTexture; \n"
                "void main() { \n"
                "vTexture = aTexture; \n"
                "gl_Position = vec4(aPosition, 1.0, 1.0); \n"
                "}"

};
static const char* FRAGMENT_SHADER = {
                "precision mediump float; \n"
                "uniform sampler2D uTexture;\n"
                        "varying vec2 vTexture; \n"
                        "void main() { \n"
                "gl_FragColor = texture2D(uTexture, vTexture); \n"
                "}"

};

struct TextureProperties {
    Resource* textureResourcePtr;
    GLuint texture;
    int32_t width;
    int32_t height;
};

class GraphicsComponent {
public:
    virtual status load() = 0;
    virtual void draw() = 0;
};

class GraphicsManager {
public:
    GraphicsManager(android_app * pApplication);
    ~GraphicsManager();
    GLuint loadShader(const char* vertexShader, const char* fragmentShader);
    void registerComponent(GraphicsComponent* graphicsComponent);
    GLuint loadVertexBuffer(const void*, int32_t size);
    GLfloat* getProjectionMatrix() { return ProjectionMatrix[0]; }
    TextureProperties* loadTexture(Resource& resource);
    int32_t getRenderWidth() const { return _RenderWidth; }
    int32_t getRenderHeight() const { return _RenderHeight; }
    int32_t getScreenWidth() const { return m_ScreenWidth; }
    int32_t getScreenHeight() const { return m_ScreenHeight; }
    status start();
    status update();
    void stop();

private:
    status initializeRenderBuffer();
    struct RenderVertex {
        GLfloat x, y, u, v;
    };
    GLuint VertexBuffers[32];
    int32_t VertexBufferCount;
    GLuint Shaders[32];
    int32_t ShadersCount;
    TextureProperties _Textures[32];
    int32_t TextureCount;
    android_app * appPtr;
    int32_t _RenderWidth, _RenderHeight;
    GraphicsComponent * _elements[32];
    int32_t _componentsCount;
    EGLDisplay display;
    EGLSurface surface;
    EGLContext eglContext;
    GLfloat ProjectionMatrix[4][4];
    int32_t m_ScreenWidth, m_ScreenHeight;

    GLint ScreenFrameBuffer;
    GLuint RenderFrameBuffer, RenderVertexBuffer;
    GLuint RenderTexture, RenderShaderProgram;
    GLuint aPosition, aTexture, uProjection, uTexture;
};


#endif //ANDROIDNATIVEBLUSTER_GRAPHICSMANAGER_H
