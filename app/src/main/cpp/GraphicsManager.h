//
// Graphic manager in app
//

#ifndef ANDROIDNATIVEBLUSTER_GRAPHICSMANAGER_H
#define ANDROIDNATIVEBLUSTER_GRAPHICSMANAGER_H

#include "Types.h"
#include "../../../../../../Library/Android/sdk/ndk-bundle/sources/android/native_app_glue/android_native_app_glue.h"

struct GraphicsElement {
    Location location;
    int32_t width, height;
    GraphicsElement(int32_t pwidth, int32_t pheight) : location(), width(pwidth), height(pheight) {}

};

class GraphicsManager {
public:
    GraphicsManager(android_app * pApplication);
    ~GraphicsManager();

    int32_t getRenderWidth() const { return _RenderWidth; }
    int32_t getRenderHeight() const { return _RenderHeight; }
    GraphicsElement * registerElement(int32_t pHeight, int32_t pWidth);
    status start();
    status update();

private:
    android_app * appPtr;
    int32_t _RenderWidth, _RenderHeight;
    GraphicsElement * _elements[1024];
    int32_t _elementsCount;

};


#endif //ANDROIDNATIVEBLUSTER_GRAPHICSMANAGER_H
