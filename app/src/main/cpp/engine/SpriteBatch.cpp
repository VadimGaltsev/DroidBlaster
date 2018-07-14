//
// Created by Vadim on 12.07.2018.
//

#include "SpriteBatch.h"
#include "../helpers/Logger.h"

SpriteBatch::SpriteBatch(TimeManager & timeManager, GraphicsManager & graphicsManager) :
        _TimeManager(timeManager),
        _GraphicsManager(graphicsManager),
        Sprites(), SpritesCount(0),
        Vertices(), VertexCount(0),
        ShaderProgram(0), Indexes(),
        IndexesCount(0), aPosition(-1),
        aTexture(-1), uProjection(-1), uTexture(-1) {
    _GraphicsManager.registerComponent(this);
}

SpriteBatch::~SpriteBatch() {
    for (int32_t i = 0; i < SpritesCount; ++i) {
        delete Sprites[i];
    }
}

Sprite* SpriteBatch::registerSprite(Resource &resourceTexture, int32_t width, int32_t height) {
    int32_t spriteCount = SpritesCount;
    int32_t index = spriteCount * 4;
    GLushort * indexes = (&Indexes[0]) + spriteCount * 6;
    Indexes[IndexesCount++] = index + 0;
    Indexes[IndexesCount++] = index + 1;
    Indexes[IndexesCount++] = index + 2;
    Indexes[IndexesCount++] = index + 2;
    Indexes[IndexesCount++] = index + 1;
    Indexes[IndexesCount++] = index + 3;
    Sprites[SpritesCount] = new Sprite(_GraphicsManager, resourceTexture, height, width);
    return Sprites[SpritesCount++];

}

status SpriteBatch::load() {
    GLuint result; int32_t spriteCount;
    ShaderProgram = _GraphicsManager.loadShader(support::VERTEX_SHADER, support::FRAGMENT_SHADER);
    if (ShaderProgram == 0)
        return STATUS_KO;
    aPosition = glGetAttribLocation(ShaderProgram, "aPosition");
    aTexture = glGetAttribLocation(ShaderProgram, "aTexture");
    uProjection = glGetUniformLocation(ShaderProgram, "uProjection");
    uTexture = glGetUniformLocation(ShaderProgram, "u_texture");
    for (int32_t i = 0; i < SpritesCount; ++i) {
        if (Sprites[i]->load(_GraphicsManager) != STATUS_OK) goto ERROR;
    }
    return STATUS_OK;
    ERROR:
    Logger::error("Error while loading sprite batch");
    return STATUS_KO;
}

void SpriteBatch::draw() {
    glUseProgram(ShaderProgram);
    glUniformMatrix4fv(uProjection, 1, GL_FALSE, _GraphicsManager.getProjectionMatrix());
    glUniform1i(uTexture, 0);
    glEnableVertexAttribArray(aPosition);
    glVertexAttribPointer(aPosition, 2, GL_FLOAT, GL_FALSE, sizeof(Sprite::Vertex), &Vertices[0].x);
    glEnableVertexAttribArray(aTexture);
    glVertexAttribPointer(aTexture, 2, GL_FLOAT, GL_FALSE, sizeof(Sprite::Vertex), &Vertices[0].u);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    const int32_t vertexPerSprite = 4;
    const int32_t indexPerSprite = 6;
    float timeStep = _TimeManager.elapsed();
    int32_t spriteCount = SpritesCount;
    int32_t currentSprite = 0, firstSprite = 0;
    while (bool canDraw = (currentSprite < spriteCount)) {
        Sprite* sprite = Sprites[currentSprite];
        GLuint currentTexture = sprite->Texture;
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, sprite->Texture);
        do {
            sprite = Sprites[currentSprite];
            if (sprite->Texture == currentTexture) {
                Sprite::Vertex* vertex = (&Vertices[currentSprite * 4]);
                sprite->draw(vertex, timeStep);
            } else break;
        } while (canDraw = (++currentSprite < spriteCount));
        glDrawElements(GL_TRIANGLES, (currentSprite - firstSprite) * indexPerSprite,
                       GL_UNSIGNED_SHORT, &Indexes[firstSprite * indexPerSprite]);
        firstSprite = currentSprite;
    }
    glUseProgram(0);
    glDisableVertexAttribArray(aPosition);
    glDisableVertexAttribArray(aTexture);
    glDisable(GL_BLEND);
}