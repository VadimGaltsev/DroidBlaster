//
// Created by Vadim on 16.07.2018.
//

#include <cstdlib>
#include "StarField.h"
#include "helpers/Logger.h"

StarField::StarField(android_app *app, TimeManager &timeManager, GraphicsManager &graphicsManager,
                     int32_t startCount, Resource &textureResource) :
        _TimeManager(timeManager),
        _GraphicsManager(graphicsManager),
        StarsCount(startCount),
        TextureResource(textureResource),
        VertexBuffer(0), Texture(-1),
        ShaderProgram(0), aPosition(-1),
        uProjection(-1), uHeight(-1),
        uTime(-1), uTexture(-1) {
    _GraphicsManager.registerComponent(this);
}

static const char* VERTEX_SHADER =
                "attribute vec4 aPosition; \n"
                "uniform mat4 uProjection; \n"
                "uniform float uHeight; \n"
                "uniform float uTime; \n"
                "void main() {; \n"
                    "const float speed = -800.0; \n"
                        "const float size = 8.0; \n"
                        "vec4 position = aPosition; \n"
                        "position.x = aPosition.x; \n"
                        "position.y = mod(aPosition.y + (uTime * speed * aPosition.z), uHeight); \n"
                        "position.z = 0.0; \n"
                        "gl_Position = uProjection * position; \n"
                        "gl_PointSize = aPosition.z * size;\n"
                        "}" ;


static const char * FRAGMENT_SHADER =
        "precision mediump float; \n"
        "uniform sampler2D uTexture; \n"
        "void main() { \n"
            "gl_FragColor = texture2D(uTexture, gl_PointCoord);\n"
                "}";

status StarField::load() {
    Logger::info("Loading stars.");
    TextureProperties* textureProperties;
    Vertex* vertexBuffer = new Vertex[StarsCount];
    for (int32_t i = 0; i < StarsCount; ++i) {
        vertexBuffer[i].x = RAND(_GraphicsManager.getRenderWidth());
        vertexBuffer[i].y = RAND(_GraphicsManager.getRenderHeight());
        vertexBuffer[i].z = RAND(1.0f);
    }
    VertexBuffer = _GraphicsManager.loadVertexBuffer((uint8_t*) vertexBuffer, StarsCount * sizeof(Vertex));
    delete[] vertexBuffer;
    if (VertexBuffer == 0) goto ERROR;
    textureProperties = _GraphicsManager.loadTexture(TextureResource);
    if (textureProperties == nullptr) goto ERROR;
    Texture = textureProperties->texture;
    ShaderProgram = _GraphicsManager.loadShader(VERTEX_SHADER, FRAGMENT_SHADER);
    if (ShaderProgram == 0) goto ERROR;
    aPosition = glGetAttribLocation(ShaderProgram, "aPosition");
    uProjection = glGetUniformLocation(ShaderProgram, "uProjection");
    uHeight = glGetUniformLocation(ShaderProgram, "uHeight");
    uTime = glGetUniformLocation(ShaderProgram, "uTime");
    uTexture = glGetUniformLocation(ShaderProgram, "uTexture");
    return STATUS_OK;

    ERROR:
    Logger::error("Error while loading stars.");
    return STATUS_KO;
}

void StarField::draw() {
    glDisable(GL_BLEND);

    glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
    glEnableVertexAttribArray(aPosition);
    glVertexAttribPointer(aPosition, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*) 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Texture);
    glUseProgram(ShaderProgram);
    glUniformMatrix4fv(uProjection, 1, GL_FALSE, _GraphicsManager.getProjectionMatrix());
    glUniform1f(uHeight, _GraphicsManager.getRenderHeight());
    glUniform1f(uTime, _TimeManager.elapsedTotal());
    glUniform1f(uTexture, 0);

    glDrawArrays(GL_POINTS, 0, StarsCount);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(0);
}