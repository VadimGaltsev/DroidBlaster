//
// Created by vadik on 12.06.2018.
//
#define ERROR_LOCK "Cannot add geometry buffer"

#include "GraphicsManager.h"
#include "../helpers/Logger.h"
#include <png.h>
#include <pngconf.h>

const int32_t DEFAULT_RENDER_WIDTH = 600;

GraphicsManager::GraphicsManager(android_app *pApplication) :
        appPtr(pApplication), _RenderWidth(0), _RenderHeight(0),
        _elements(), eglContext(EGL_NO_CONTEXT), display(EGL_NO_DISPLAY), surface(EGL_NO_SURFACE),
        TextureCount(0), _Textures(), ProjectionMatrix(), Shaders(),
        ShadersCount(0),_componentsCount(0), VertexBuffers(),
        VertexBufferCount(0), ScreenFrameBuffer(0),
        RenderFrameBuffer(0), RenderVertexBuffer(0),
        RenderTexture(0), RenderShaderProgram(0),
        aPosition(0), aTexture(0), uProjection(0),
        uTexture(0) {
    Logger::info("Creating graphicManager");
}

status GraphicsManager::initializeRenderBuffer() {
    Logger::info("Loading offscreen buffer.");
    const RenderVertex vertices[] = {
            {-1.0f, -1.0f, 0.0f, 0.0f},
            {-1.0f, 1.0f, 0.0f, 1.0f},
            {1.0f, -1.0f, 1.0f, 0.0f},
            {1.0f, 1.0f, 1.0f, 1.0f},
    };
    float screenRatio = float(m_ScreenHeight) / float(m_ScreenWidth);
    _RenderWidth = DEFAULT_RENDER_WIDTH;
    _RenderHeight = float(_RenderWidth) * screenRatio;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &ScreenFrameBuffer);
    glGenTextures(1, &RenderTexture);
    glBindTexture(GL_TEXTURE_2D, RenderTexture);
    //texture settings
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _RenderWidth, _RenderHeight, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL);
    glGenFramebuffers(1, &RenderFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, RenderFrameBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, RenderTexture, 0);
    glBindTexture(GL_TEXTURE_2D, 0); glBindFramebuffer(GL_FRAMEBUFFER, 0);
    RenderVertexBuffer = loadVertexBuffer(vertices, sizeof(vertices));
    if (RenderVertexBuffer == 0 ) goto ERROR;
    RenderShaderProgram = loadShader(VERTEX_SHADER, FRAGMENT_SHADER);
    if (RenderShaderProgram == 0) goto ERROR;
    aPosition = glGetAttribLocation(RenderShaderProgram, "aPosition");
    aTexture = glGetAttribLocation(RenderShaderProgram, "aTexture");
    uTexture = glGetUniformLocation(RenderShaderProgram, "uTexture");
    return STATUS_OK;
    ERROR:
    Logger::error("Error while loading offsreen buffer.");
    return STATUS_KO;
}

void GraphicsManager::registerComponent(GraphicsComponent *graphicsComponent) {
    _elements[_componentsCount++] = graphicsComponent;
}

GraphicsManager::~GraphicsManager() {
    Logger::info("Destroying process started");
//    for (int32_t i = 0; i < _componentsCount; ++i) {
//        delete _elements[i];
//    }
}


status GraphicsManager::start() {
    Logger::info("Starting graphic");
    EGLint format, numConfigs, errorResult;
    GLenum status;
    EGLConfig eglConfig;
    const EGLint DISPLAY_ATTRIBS[] = {EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
    EGL_BLUE_SIZE, 5, EGL_GREEN_SIZE, 6, EGL_RED_SIZE, 5,
    EGL_SURFACE_TYPE, EGL_WINDOW_BIT, EGL_NONE};
    const EGLint CONTEXT_ATTRIBS[] = {
            EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE
    };
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (display == EGL_NO_DISPLAY) goto ERROR;
    if (!eglInitialize(display, NULL, NULL)) goto ERROR;
    if (!eglChooseConfig(display, DISPLAY_ATTRIBS, &eglConfig, 1, &numConfigs) || numConfigs <= 0) goto ERROR;
    if (!eglGetConfigAttrib(display, eglConfig, EGL_NATIVE_VISUAL_ID, &format)) goto ERROR;
    ANativeWindow_setBuffersGeometry(appPtr->window, 0, 0, format);
    surface = eglCreateWindowSurface(display, eglConfig, appPtr->window, NULL);
    if (surface == EGL_NO_SURFACE) goto ERROR;
    eglContext = eglCreateContext(display, eglConfig, NULL, CONTEXT_ATTRIBS);
    if (eglContext == EGL_NO_CONTEXT) goto ERROR;
    if (!eglMakeCurrent(display, surface, surface, eglContext) ||
            !eglQuerySurface(display, surface, EGL_WIDTH, &m_ScreenWidth) ||
            !eglQuerySurface(display, surface, EGL_HEIGHT, &m_ScreenHeight) ||
            m_ScreenWidth <= 0 || m_ScreenHeight <= 0) goto ERROR;
    if (initializeRenderBuffer() != STATUS_OK) goto ERROR;
    glViewport(0,0, _RenderWidth, _RenderHeight);
    glDisable(GL_DEPTH_TEST);
    memset(ProjectionMatrix[0], 0, sizeof(ProjectionMatrix));
    ProjectionMatrix[0][0] = 2.0f / GLfloat(_RenderWidth);
    ProjectionMatrix[1][1] = 2.0f / GLfloat(_RenderHeight);
    ProjectionMatrix[2][2] = -1.0f; ProjectionMatrix[3][0] = -1.0f;
    ProjectionMatrix[3][1] = -1.0f; ProjectionMatrix[3][2] = 0.0f;
    ProjectionMatrix[3][3] = 1.0f;
    for (int32_t i = 0; i < _componentsCount; ++i) {
        if (_elements[i]->load() != STATUS_OK) {
            return STATUS_KO;
        }
    }
    return STATUS_OK;
    ERROR:
        Logger::error("Error while starting Graphics manager");
        stop();
        return STATUS_KO;
//    ANativeWindow_Buffer aNativeWindow_buffer;
//    if (ANativeWindow_setBuffersGeometry(appPtr->window, 0, 0, WINDOW_FORMAT_RGBX_8888) < 0) {
//        Logger::error(ERROR_LOCK);
//        return STATUS_KO;
//    }
//    if (ANativeWindow_lock(appPtr->window, &aNativeWindow_buffer, NULL) >= 0) {
//        _RenderWidth = aNativeWindow_buffer.width;
//        _RenderHeight = aNativeWindow_buffer.height;
//        ANativeWindow_unlockAndPost(appPtr->window);
//    } else {
//        Logger::error("Error while locking window");
//        return STATUS_KO;
//    }
//    return STATUS_OK;
}

GLuint GraphicsManager::loadShader(const char *_vertexShader, const char *_fragmentShader) {
    GLint result; char log[256];
    GLuint vertexShader, fragmentShader, shaderProgram;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &_vertexShader, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        glGetShaderInfoLog(vertexShader, sizeof(log), 0, log);
        Logger::error("Vertex shader error: %s", log);
        goto ERROR;
    }
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1 , &_fragmentShader, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        glGetShaderInfoLog(fragmentShader, sizeof(log), 0, log);
        Logger::error("Fragment shader error: %s", log);
        goto ERROR;
    }
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    if (result == GL_FALSE) {
        glGetShaderInfoLog(shaderProgram, sizeof(log), 0, log);
        Logger::error("Program shader error: %s", log);
        goto ERROR;
    }
    Shaders[ShadersCount++] = shaderProgram;
    return shaderProgram;
    ERROR:
    Logger::error("Error while loading shader");
    if (vertexShader > 0) glDeleteShader(vertexShader);
    if (fragmentShader > 0) glDeleteShader(fragmentShader);
    return 0;
}

status GraphicsManager::update() {
//    static float clearColor = 0.0f;
//    clearColor += 0.001f;
//    glClearColor(clearColor, clearColor, clearColor, 1.0f);
    glBindFramebuffer(GL_FRAMEBUFFER, RenderFrameBuffer);
    glViewport(0,0, _RenderWidth, _RenderHeight);
    glClear(GL_COLOR_BUFFER_BIT);
    for (int32_t i = 0; i < _componentsCount; ++i) {
        _elements[i]->draw();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, ScreenFrameBuffer);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0,0, m_ScreenWidth, m_ScreenHeight);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, RenderTexture);
    glUseProgram(RenderShaderProgram);
    glUniform1i(uTexture, 0);
    glBindBuffer(GL_ARRAY_BUFFER, RenderVertexBuffer);
    glEnableVertexAttribArray(aPosition);
    glVertexAttribPointer(aPosition, 2, GL_FLOAT, GL_FALSE, sizeof(RenderVertex), (GLvoid*) 0);
    glEnableVertexAttribArray(aTexture);
    glVertexAttribPointer(aTexture, 2, GL_FLOAT, GL_FALSE, sizeof(RenderVertex), (GLvoid*) (sizeof(GLfloat) * 2));
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    if (eglSwapBuffers(display, surface) != EGL_TRUE) {
        Logger::error("Error %d swapping buffers.", eglGetError());
        return STATUS_KO;
    } else return STATUS_OK;

//    ANativeWindow_Buffer window_buffer;
//    if (ANativeWindow_lock(appPtr->window, &window_buffer, NULL) < 0) {
//        Logger::error(ERROR_LOCK);
//        return STATUS_KO;
//    }
//    memset(window_buffer.bits, 0, window_buffer.stride * window_buffer.height * sizeof(uint32_t*));
//    //display graphics elements
//    int32_t maxX = window_buffer.width - 1;
//    int32_t maxY = window_buffer.height - 1;
//    for (int32_t i = 0; i < _elementsCount; ++i) {
//        GraphicsElement * element = _elements[i];
//        //take coordinates
//        int32_t leftX = static_cast<int32_t>(element->location.x - element->width / 2);
//        int32_t rightX = static_cast<int32_t>(element->location.x + element->width / 2);
//        int32_t leftY = static_cast<int32_t>(window_buffer.height - element->location.y - element->height / 2);
//        int32_t rightY = static_cast<int32_t>(window_buffer.height - element->location.y + element->height / 2);
//        //cuts coordinates
//        if (rightX < 0 || leftX > maxX || rightY < 0 || leftY > maxY) continue;
//        if (leftX < 0) leftX = 0;
//        else if (rightX > maxX) rightX = maxX;
//        if (leftY < 0) leftY = 0;
//        else if (rightY > maxY) rightY = maxY;
//        uint32_t * line = ((uint32_t*)(window_buffer.bits)) + (window_buffer.stride * leftY);
//        for (int iY = leftY; iY <= rightY; ++iY) {
//            for (int iX = leftX; iX < rightX; ++iX) {
//                line[iX] = 0x000000FF;
//            }
//            line = line + window_buffer.stride;
//        }
//    }
//    ANativeWindow_unlockAndPost(appPtr->window);
//    return STATUS_OK;
}


TextureProperties* GraphicsManager::loadTexture(Resource &resource) {
    for (int32_t i = 0; i < TextureCount; ++i) {
        if (resource == *_Textures[i].textureResourcePtr) {
            Logger::info("Found %s in cache", resource.getPath());
            return &_Textures[i];
        }
    }
    Logger::info("Loading texture %s", resource.getPath());
    TextureProperties* properties;
    GLuint texture; GLint format;
    png_byte header[8];
    png_structp pngPtr = nullptr; png_infop infoPtr = nullptr; png_byte * image = nullptr; png_bytep* rowPtrs = nullptr;
    png_int_32  rowSize;
    bool transparency;
    if (resource.open() != STATUS_OK) goto ERROR;
    Logger::info("Checking signature");
    if (resource.read(header, sizeof(header)) != STATUS_OK) goto ERROR;
    if (png_sig_cmp(header, 0, 8) != 0) goto ERROR;
    Logger::info("Creating required structures");
    pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!pngPtr) goto ERROR;
    infoPtr = png_create_info_struct(pngPtr);
    if (!infoPtr) goto ERROR;
    png_set_read_fn(pngPtr, &resource, support::callBackReadPng);
    if(setjmp(png_jmpbuf(pngPtr))) goto ERROR;
    png_set_sig_bytes(pngPtr, 8);
    png_read_info(pngPtr, infoPtr);
    png_int_32  depth, colorType;
    png_uint_32 width, height;
    png_get_IHDR(pngPtr, infoPtr, &width, &height, &depth, &colorType, NULL, NULL, NULL);
    transparency = false;
    if (png_get_valid(pngPtr, infoPtr, PNG_INFO_tRNS)) {
        png_set_tRNS_to_alpha(pngPtr);
        transparency = true;
    }
    if (depth < 8) {
        png_set_packing(pngPtr);
    } else if (depth == 16) {
        png_set_strip_16(pngPtr);
    }
    switch (colorType) {
        case PNG_COLOR_TYPE_PALETTE:
            png_set_palette_to_rgb(pngPtr);
            format = transparency ? GL_RGBA : GL_RGB;
            break;
        case PNG_COLOR_TYPE_RGB:
            format = transparency ? GL_RGBA : GL_RGB;
            break;
        case PNG_COLOR_TYPE_RGBA:
            format = GL_RGBA;
            break;
        case PNG_COLOR_TYPE_GRAY:
            png_set_expand_gray_1_2_4_to_8(pngPtr);
            format = transparency ? GL_LUMINANCE_ALPHA : GL_LUMINANCE;
            break;
        case PNG_COLOR_TYPE_GA:
            png_set_expand_gray_1_2_4_to_8(pngPtr);
            format = GL_LUMINANCE_ALPHA;
            break;
    }
    png_read_update_info(pngPtr, infoPtr);
    rowSize = static_cast<png_int_32>(png_get_rowbytes(pngPtr, infoPtr));
    if (rowSize <= 0) goto ERROR;
    image = new png_byte[rowSize * height];
    if (!image) goto ERROR;
    rowPtrs = new png_bytep[height];
    if (!rowPtrs) goto ERROR;
    for (int32_t i = 0; i < height; ++i) {
        rowPtrs[height - (i + 1)] = image + i * rowSize;
    }
    png_read_image(pngPtr, rowPtrs);
    resource.close();
    png_destroy_read_struct(&pngPtr, &infoPtr, NULL);
    delete [] rowPtrs;
    GLenum errorResult;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    //texture settings
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
    glBindTexture(GL_TEXTURE_2D, 0);
    delete[] image;
    if (glGetError() != GL_NO_ERROR) goto ERROR;
    Logger::info("Texture size: %d x %d", width, height);
    //caching
    properties = &_Textures[TextureCount++];
    properties->texture = texture;
    properties->textureResourcePtr = &resource;
    properties->width = width;
    properties->height = height;
    return properties;
    //error go to label!
    ERROR:
    Logger::error("Error while loading texture");
    resource.close();
    delete[] rowPtrs; delete [] image;
    if (pngPtr != nullptr) {
        png_infop * pngInfo = infoPtr != nullptr ? &infoPtr : NULL;
        png_destroy_read_struct(&pngPtr, pngInfo, NULL);
    }
    return NULL;
}

void GraphicsManager::stop() {
    Logger::info("Stopping Graphic Manager");
    for (int32_t i = 0; i < TextureCount; ++i)
    glDeleteTextures(1, &_Textures[i].texture);
    TextureCount = 0;
    for (int32_t i = 0; i < ShadersCount; ++i) {
        glDeleteShader(Shaders[i]);
    }
    for (int32_t i = 0; i < VertexBufferCount; ++i) {
        glDeleteBuffers(1, &VertexBuffers[i]);
    }
    VertexBufferCount = 0;
    ShadersCount = 0;
    if (RenderFrameBuffer != 0) {
        glDeleteFramebuffers(1, &RenderFrameBuffer);
        RenderFrameBuffer = 0;
    }
    if (RenderTexture != 0) {
        glDeleteTextures(1, &RenderTexture);
        RenderTexture = 0;
    }
    if (display != EGL_NO_DISPLAY) {
        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (eglContext != EGL_NO_CONTEXT) {
            eglDestroyContext(display, eglContext);
            eglContext = EGL_NO_CONTEXT;
        }
        if (surface != EGL_NO_SURFACE) {
            eglDestroySurface(display, surface);
            surface = EGL_NO_SURFACE;
        }
        eglTerminate(display);
        display = EGL_NO_DISPLAY;
    }
}

void support::callBackReadPng(png_structp structp, png_bytep data, png_size_t size) {
    Resource * resource = (Resource*) png_get_io_ptr(structp);
    if (resource->read(data, size) != STATUS_OK) {
        resource->close();
    }
}

GLuint GraphicsManager::loadVertexBuffer(const void * p_VertexBuffer, int32_t size) {
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, size, p_VertexBuffer, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    if (glGetError() != GL_NO_ERROR) goto ERROR;
    VertexBuffers[VertexBufferCount++] = vertexBuffer;
    return vertexBuffer;
    ERROR:
        Logger::error("Error while loading vertex buffer");
        if (vertexBuffer > 0) glDeleteBuffers(1, &vertexBuffer);
        return 0;
}