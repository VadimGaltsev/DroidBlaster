//
// Created by Vadim on 18.07.2018.
//

#include <cmath>
#include "InputManager.h"
#include "../helpers/Logger.h"

InputManager::InputManager(android_app * app, GraphicsManager & graphicsManager) :
        AndroidApp(app), _GraphicsManager(graphicsManager),
        DirectionX(0.0f), DirectionY(0.0f),
        RefPoint(nullptr) {

}

void InputManager::start() {
    Logger::info("Starting InputManager.");
    DirectionX = 0.0f; DirectionY = 0.0f;
    ScaleFactor = float(_GraphicsManager.getRenderWidth()) / float(_GraphicsManager.getScreenWidth());
}

bool InputManager::onTouchEvent(AInputEvent * event) {
    static const float TOUCH_MAX_RANGE = 65.0f;

    if (RefPoint != nullptr) {
        if (AMotionEvent_getAction(event) == AMOTION_EVENT_ACTION_MOVE) {
            float x = AMotionEvent_getX(event, 0) * ScaleFactor;
            float y = (float(_GraphicsManager.getScreenHeight()) - AMotionEvent_getY(event, 0)) * ScaleFactor;
            float moveX = x - RefPoint->x;
            float moveY = y - RefPoint->y;
            float moveRange = sqrt((moveX * moveX) + (moveY * moveY));
            if (moveRange > TOUCH_MAX_RANGE) {
                float cropFactor = TOUCH_MAX_RANGE / moveRange;
                moveX *= cropFactor;
                moveY *= cropFactor;
            }
            DirectionX = moveX / TOUCH_MAX_RANGE;
            DirectionY = moveY / TOUCH_MAX_RANGE;
            Logger::info("New directions x: %f , y: %f , ScaleFactor: %f", DirectionX, DirectionY, ScaleFactor);
        }
    } else { DirectionX = 0.0f; DirectionY = 0.0f; }
    return true;
}