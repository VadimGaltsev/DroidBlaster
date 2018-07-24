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
    Configure configure(app);
    _Rotation = configure.getRotation();
}

bool InputManager::onAccelerometerEvent(ASensorEvent *event) {
    static const float GRAVITY = ASENSOR_STANDARD_GRAVITY / 2.0f;
    static const float MIN_X = -1.0f; static const float MAX_X = 1.0f;
    static const float MIN_Y = -1.0f; static const float MAX_Y = 1.0f;
    static const float CENTER_X = (MIN_X + MAX_X) / 2.0f;
    static const float CENTER_Y = (MAX_Y + MIN_Y) / 2.0f ;
    ASensorVector vector;
    toScreenCoord(_Rotation, &event->vector, &vector);

    float rawHorizontal = event->vector.x / GRAVITY;
    if (rawHorizontal > MAX_X) {
        rawHorizontal = MAX_X;
    } else if (rawHorizontal < MIN_X) {
        rawHorizontal = MIN_X;
    }
    DirectionX = CENTER_X - rawHorizontal;

    float rawVertical = event->vector.y / GRAVITY;

    if (rawVertical > MAX_Y) {
        rawVertical = MAX_Y;
    } else if (rawVertical < MIN_Y) {
        rawVertical = MIN_Y;
    }
    DirectionY = CENTER_Y - rawVertical;
    return true;
}

void InputManager::toScreenCoord(screen_rotation pSr, ASensorVector * pASVect, ASensorVector * pVectScreen) {
    struct AxisSwap {
        int8_t negX; int8_t negY;
        int8_t xSrc; int8_t ySrc;
    };

    static const AxisSwap axisSwaps[] = {
            {1, -1, 0, 1}, //rot 0
            {-1, -1, 1, 0}, //rot 90
            {-1, 1, 0, 1}, //rot 180
            {1, 1, 1, 0} // rot 270
    };
    const AxisSwap& swap = axisSwaps[pSr];
    pVectScreen->v[0] = swap.negX * pASVect->v[swap.xSrc];
    pVectScreen->v[1] = swap.negY * pASVect->v[swap.ySrc];
    pVectScreen->v[2] = pASVect->v[2];
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

