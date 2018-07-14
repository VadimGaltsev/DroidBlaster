//
// Created by vadik on 06.07.2018.
//

#ifndef ANDROIDNATIVEBLUSTER_PHYSICSBODY_H
#define ANDROIDNATIVEBLUSTER_PHYSICSBODY_H

#include "../Types.h"
#include "TimeManager.h"
#include "GraphicsManager.h"

struct PhysicsBody {
    PhysicsBody(Location* locationPtr, int32_t _width, int32_t _height) :
            location(locationPtr), width(_width), height(_height),
            velocityX(0.0f), velocityY(0.0f) {}
    Location* location;
    int32_t width, height;
    float velocityX, velocityY;
};
class PhysicsManager {
public:
    PhysicsManager(TimeManager& _TimerManager, GraphicsManager& _GraphicsManager);
    ~PhysicsManager();
    PhysicsBody* loadBody(Location& location, int32_t _width, int32_t height);
    void update();

private:
    TimeManager& _TimeManager;
    GraphicsManager& graphicsManager;
    PhysicsBody* _PhysicsBodies[1024];
    int32_t _PhysicsBodyCount;
};
#endif //ANDROIDNATIVEBLUSTER_PHYSICSBODY_H
