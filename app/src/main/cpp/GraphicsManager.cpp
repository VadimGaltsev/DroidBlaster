//
// Created by vadik on 12.06.2018.
//
#define ERROR_LOCK "Cannot add geometry buffer"

#include "GraphicsManager.h"
#include "Logger.h"

GraphicsManager::GraphicsManager(android_app *pApplication) : appPtr(pApplication), _RenderWidth(0), _RenderHeight(0),
    _elementsCount(0), _elements() {
    Logger::info("Creating graphicManager");
}

GraphicsManager::~GraphicsManager() {
    Logger::info("Destroying process started");
    for (int32_t i = 0; i < _elementsCount; ++i) {
        delete _elements[i];
    }
}

GraphicsElement* GraphicsManager::registerElement(int32_t pHeight, int32_t pWidth) {
    _elements[_elementsCount] = new GraphicsElement(pHeight, pWidth);
    return _elements[_elementsCount++];
}

status GraphicsManager::start() {
    Logger::info("Starting graphic");
    ANativeWindow_Buffer aNativeWindow_buffer;
    if (ANativeWindow_setBuffersGeometry(appPtr->window, 0, 0, WINDOW_FORMAT_RGBX_8888) < 0) {
        Logger::error(ERROR_LOCK);
        return STATUS_KO;
    }
    if (ANativeWindow_lock(appPtr->window, &aNativeWindow_buffer, NULL) >= 0) {
        _RenderWidth = aNativeWindow_buffer.width;
        _RenderHeight = aNativeWindow_buffer.height;
        ANativeWindow_unlockAndPost(appPtr->window);
    } else {
        Logger::error("Error while locking window");
        return STATUS_KO;
    }
    return STATUS_OK;
}

status GraphicsManager::update() {
    ANativeWindow_Buffer window_buffer;
    if (ANativeWindow_lock(appPtr->window, &window_buffer, NULL) < 0) {
        Logger::error(ERROR_LOCK);
        return STATUS_KO;
    }
    memset(window_buffer.bits, 0, window_buffer.stride * window_buffer.height * sizeof(uint32_t*));
    //display graphics elements
    int32_t maxX = window_buffer.width - 1;
    int32_t maxY = window_buffer.height - 1;
    for (int32_t i = 0; i < _elementsCount; ++i) {
        GraphicsElement * element = _elements[i];
        //take coordinates
        int32_t leftX = static_cast<int32_t>(element->location.x - element->width / 2);
        int32_t rightX = static_cast<int32_t>(element->location.x + element->width / 2);
        int32_t leftY = static_cast<int32_t>(window_buffer.height - element->location.y - element->height / 2);
        int32_t rightY = static_cast<int32_t>(window_buffer.height - element->location.y + element->height / 2);
        //cuts coordinates
        if (rightX < 0 || leftX > maxX || rightY < 0 || leftY > maxY) continue;
        if (leftX < 0) leftX = 0;
        else if (rightX > maxX) rightX = maxX;
        if (leftY < 0) leftY = 0;
        else if (rightY > maxY) rightY = maxY;
        uint32_t * line = ((uint32_t*)(window_buffer.bits)) + (window_buffer.stride * leftY);
        for (int iY = leftY; iY <= rightY; ++iY) {
            for (int iX = leftX; iX < rightX; ++iX) {
                line[iX] = 0x000000FF;
            }
            line = line + window_buffer.stride;
        }
    }
    ANativeWindow_unlockAndPost(appPtr->window);
    return STATUS_OK;
}
