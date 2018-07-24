//
// Created by Vadim on 18.07.2018.
//

#ifndef DROIDBLASTER_INPUTHANDLER_H
#define DROIDBLASTER_INPUTHANDLER_H

#include <android/input.h>
#include <android/sensor.h>

class InputHandler {
public:
    virtual ~InputHandler(){}
    virtual bool onTouchEvent(AInputEvent*) = 0;
    virtual bool onAccelerometerEvent(ASensorEvent*) = 0;
};


#endif //DROIDBLASTER_INPUTHANDLER_H
