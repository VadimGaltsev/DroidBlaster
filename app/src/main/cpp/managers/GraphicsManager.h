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
    GLfloat* getProjectionMatrix() { return ProjectionMatrix[0]; }
    TextureProperties* loadTexture(Resource& resource);
    int32_t getRenderWidth() const { return _RenderWidth; }
    int32_t getRenderHeight() const { return _RenderHeight; }
    status start();
    status update();
    void stop();

private:
    struct RenderVertex {
        GLfloat x, y, u, v;
    };
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
};


#endif //ANDROIDNATIVEBLUSTER_GRAPHICSMANAGER_H
