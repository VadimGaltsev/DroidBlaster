//
// Created by Vadim on 12.07.2018.
//

#ifndef DROIDBLASTER_SPRITEBATCH_H
#define DROIDBLASTER_SPRITEBATCH_H

#include "../managers/TimeManager.h"
#include "Sprite.h"
#include "../Types.h"
#include "../managers/GraphicsManager.h"
namespace support {
    static const char * VERTEX_SHADER = {
                    "attribute vec4 aPosition; \n"
                    "attribute vec2 aTexture; \n"
                    "varying vec2 vTexture; \n"
                    "uniform mat4 uProjection; \n"
                    "void main() {; \n"
                    "vTexture = aTexture; \n"
                    "gl_Position = uProjection * aPosition; \n"
                    "}"

    };
    static const char* FRAGMENT_SHADER = {
            "precision mediump float; \n"
                    "varying vec2 vTexture; \n"
                    "uniform sampler2D u_texture;\n"
                    "void main() {; \n"
                    "gl_FragColor = texture2D(u_texture, vTexture); \n"
                    "}"

    };
}

class SpriteBatch : public GraphicsComponent {
public:
    SpriteBatch(TimeManager& timeManager, GraphicsManager& graphicsManager);
    ~SpriteBatch();
    Sprite* registerSprite(Resource& resourceTexture, int32_t width, int32_t height);
    status load();
    void draw();

private:
    TimeManager& _TimeManager;
    GraphicsManager& _GraphicsManager;

    Sprite* Sprites[1024]; int32_t SpritesCount;
    Sprite::Vertex Vertices[1024]; int32_t VertexCount;
    GLushort Indexes[1024]; int32_t IndexesCount;
    GLuint ShaderProgram;
    GLuint aPosition; GLuint aTexture;
    GLuint uProjection; GLuint uTexture;
};


#endif //DROIDBLASTER_SPRITEBATCH_H
