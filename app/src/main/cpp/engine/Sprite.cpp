//
// Created by Vadim on 12.07.2018.
//

#include "Sprite.h"
#include "../helpers/Logger.h"

Sprite::Sprite(GraphicsManager &graphicsManager, Resource &textureRes, int32_t height,
               int32_t width) : location(), textureRes(textureRes), Texture(0), SheetWidth(0),
                                SheetHeight(0), SpriteHeight(height),
                                SpriteWidth(width), FrameCount(0),
                                FrameXCount(0), FrameYCount(0),
                                AnimationLoop(false), AnimationFrameCount(1),
                                AnimationFrame(0), AnimationSpeed(0)  { }

status Sprite::load(GraphicsManager &graphicsManager) {
    TextureProperties* properties = graphicsManager.loadTexture(textureRes);
    if (properties == NULL) return  STATUS_KO;
    Texture = properties->texture;
    SheetHeight = properties->height;
    SheetWidth = properties->width;
    FrameXCount = SheetWidth / SpriteWidth;
    FrameYCount = SheetHeight / SpriteHeight;
    FrameCount = FrameYCount * FrameXCount;
    return STATUS_OK;
}

void Sprite::setAnimation(int32_t startFrame, int32_t frameCount,
                          float speed, bool loop) {
    AnimationStartFrame = startFrame;
    AnimationFrame = 0.0f; AnimationSpeed = speed, AnimationLoop = loop;
    AnimationFrameCount = frameCount;
}

void Sprite::draw(Vertex vertex[4], float timeStep) {
    int32_t currentFrame, currentFrameX, currentFrameY;
    AnimationFrame += timeStep * AnimationSpeed;
    if (AnimationLoop) {
        currentFrame = (AnimationStartFrame + int32_t(AnimationFrame)) % AnimationFrameCount;
    } else {
        if (endAnimation())
            currentFrame = AnimationStartFrame + AnimationFrameCount - 1;
        else
            currentFrame = AnimationStartFrame + int32_t(AnimationFrame);
    }
    currentFrameX = currentFrame % FrameXCount;
    currentFrameY = FrameYCount - 1 - (currentFrame / FrameXCount);

    GLfloat posX1 = location.x - float(SpriteWidth >> 1);
    GLfloat posY1 = location.y - float(SpriteHeight >> 1);
    GLfloat posX2 = posX1 + SpriteWidth;
    GLfloat posY2 = posY1 + SpriteHeight;
    GLfloat u1 = GLfloat(currentFrameX * SpriteWidth) / GLfloat(SheetWidth);
    GLfloat u2 = GLfloat((currentFrameX + 1) * SpriteWidth) / GLfloat(SheetWidth);
    GLfloat v1 = GLfloat(currentFrameY * SpriteHeight) / GLfloat(SheetHeight);
    GLfloat v2 = GLfloat((currentFrameY + 1) * SpriteHeight) / GLfloat(SheetHeight);
    vertex[0].x = posX1; vertex[0].y = posY1;
    vertex[0].u = u1; vertex[0].v = v1;
    vertex[1].x = posX1; vertex[1].y = posY2;
    vertex[1].u = u1; vertex[1].v = v2;
    vertex[2].x = posX2; vertex[2].y = posY1;
    vertex[2].u = u2; vertex[2].v = v1;
    vertex[3].x = posX2; vertex[3].y = posY2;
    vertex[3].u = u2; vertex[3].v = v2;

}