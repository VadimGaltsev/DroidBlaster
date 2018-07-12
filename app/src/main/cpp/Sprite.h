//
// Created by Vadim on 12.07.2018.
//

#ifndef DROIDBLASTER_SPRITE_H
#define DROIDBLASTER_SPRITE_H

#include "Resource.h"
#include "GraphicsManager.h"
#include "Types.h"

#include <GLES2/gl2.h>

class SpriteBatch;

class Sprite {
    friend class SpriteBatch;

public:
    struct Vertex {
        GLfloat x, y, u, v;
    };
    Sprite(GraphicsManager& graphicsManager, Resource& textureRes,
           int32_t height, int32_t width);
    void setAnimation(int32_t startFrame, int32_t frameCount, float speed, bool loop);
    bool endAnimation() const { return AnimationFrame > (AnimationFrameCount - 1); }
    Location location;
protected:
    status load(GraphicsManager& graphicsManager);
    void draw(Vertex vertex[4], float timeStep);
private:
    Resource& textureRes;
    GLuint Texture;
    int32_t SheetHeight, SheetWidth;
    int32_t SpriteHeight, SpriteWidth;
    int32_t FrameXCount, FrameYCount, FrameCount;

    int32_t AnimationStartFrame, AnimationFrameCount;
    float AnimationSpeed, AnimationFrame;
    bool AnimationLoop;
};


#endif //DROIDBLASTER_SPRITE_H
