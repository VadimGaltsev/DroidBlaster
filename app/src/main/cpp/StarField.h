//
// Created by Vadim on 16.07.2018.
//

#ifndef DROIDBLASTER_STARFIELD_H
#define DROIDBLASTER_STARFIELD_H


#include "managers/GraphicsManager.h"
#include "managers/TimeManager.h"

class StarField : GraphicsComponent {
public:
    StarField(android_app * app, TimeManager& timeManager,
    GraphicsManager& graphicsManager, int32_t startCount, Resource& textureResource);
    status load();
    void draw();

private:
    struct Vertex {
        GLfloat x, y, z;
    };
    TimeManager& _TimeManager;
    GraphicsManager& _GraphicsManager;
    int32_t  StarsCount;
    Resource& TextureResource;
    GLuint VertexBuffer, Texture, ShaderProgram,
    aPosition, uProjection, uTime, uHeight, uTexture;
};


#endif //DROIDBLASTER_STARFIELD_H
