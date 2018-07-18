//
// Created by Vadim on 18.07.2018.
//

#ifndef DROIDBLASTER_INPUTMANAGER_H
#define DROIDBLASTER_INPUTMANAGER_H

#include "GraphicsManager.h"
#include "../handlers/InputHandler.h"

class InputManager : public InputHandler {
public:
    InputManager(android_app*, GraphicsManager&);
    float getDirectionX() const { return DirectionX; }
    float getDirectionY() const { return DirectionY; }
    void setRefPoint(Location* location) { RefPoint = location; }
    void start();

protected:
    bool onTouchEvent(AInputEvent*);

private:
    android_app* AndroidApp;
    GraphicsManager& _GraphicsManager;
    float ScaleFactor;
    float DirectionX;
    float DirectionY;
    Location* RefPoint;
};


#endif //DROIDBLASTER_INPUTMANAGER_H
