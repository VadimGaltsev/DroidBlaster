//
// Created by vadik on 06.07.2018.
//

#ifndef ANDROIDNATIVEBLUSTER_PHYSICSBODY_H
#define ANDROIDNATIVEBLUSTER_PHYSICSBODY_H

#include "../Types.h"
#include "TimeManager.h"
#include "GraphicsManager.h"
#include <vector>
#include <>
#define PHYSICS_SCALE 32.0f

struct PhysicsCollision {
    bool collide;
    PhysicsCollision() : collide(false) {}
};

struct PhysicsBody {
    PhysicsBody(Location* locationPtr, int32_t _width, int32_t _height) :
            location(locationPtr), width(_width), height(_height),
            velocityX(0.0f), velocityY(0.0f) {}
    Location* location;
    int32_t width, height;
    float velocityX, velocityY;
};
class PhysicsManager : private b2ContactListener {
public:
    PhysicsManager(TimeManager& _TimerManager, GraphicsManager& _GraphicsManager);
    ~PhysicsManager();
    b2Body* loadBody(Location& location, uint16, uint16, int32_t, int32_t, float);
    b2MouseJoint* loadTarget(b2Body*);
    void update();
    void start();
private:
    void BeginContact(b2Contact*);
    std::vector<b2Body*> Bodies;
    std::vector<Location*> Locations;
    b2Body* BodyBoundsObj;
    b2World world;
    TimeManager& _TimeManager;
    GraphicsManager& graphicsManager;
    PhysicsBody* _PhysicsBodies[1024];
    int32_t _PhysicsBodyCount;
};
#endif //ANDROIDNATIVEBLUSTER_PHYSICSBODY_H
