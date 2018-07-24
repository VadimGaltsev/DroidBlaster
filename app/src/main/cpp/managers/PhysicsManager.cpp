//
// Created by vadik on 06.07.2018.
//

#include "PhysicsManager.h"
#include "../helpers/Logger.h"
#include <>
static const int32_t VELOCITY_ITER = 6;
static const int32_t POSITION_ITER = 2;

PhysicsManager::PhysicsManager(TimeManager &_TimerManager, GraphicsManager &_GraphicsManager) :
                _TimeManager(_TimerManager), graphicsManager(_GraphicsManager),
                _PhysicsBodies(), _PhysicsBodyCount(0), world(b2Vec2_zero), Bodies(), Locations(), BodyBoundsObj(NULL) {
    Logger::info("Creating PhysicsManager");
    world.SetContactListener(this);
}

PhysicsManager::~PhysicsManager() {
    Logger::info("Destroying PhysicsManager");
    std::vector<b2Body*>::iterator bodyIt;
    for (bodyIt = Bodies.begin(); bodyIt < Bodies.end(); ++bodyIt) {
        delete (PhysicsCollision*) *(bodyIt)->GetUserData();
    }
}

b2Body* PhysicsManager::loadBody(Location &location, uint16 pCategoty,
                                 uint16 pMask, int32_t pSizex, int32_t pSizeY, float pRestitution) {}

void PhysicsManager::update() {
    float timeStamp = _TimeManager.elapsed();
    for (int32_t j = 0; j < _PhysicsBodyCount; ++ j) {
        PhysicsBody* body = _PhysicsBodies[j];
        body->location->x += (timeStamp * body->velocityX);
        body->location->y += (timeStamp * body->velocityY);
    }
}