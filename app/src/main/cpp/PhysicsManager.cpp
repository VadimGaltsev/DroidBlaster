//
// Created by vadik on 06.07.2018.
//

#include "PhysicsManager.h"
#include "Logger.h"

PhysicsManager::PhysicsManager(TimeManager &_TimerManager, GraphicsManager &_GraphicsManager) :
                _TimeManager(_TimerManager), graphicsManager(_GraphicsManager),
                _PhysicsBodies(), _PhysicsBodyCount(0) {
    Logger::info("Creating PhysicsManager");
}

PhysicsManager::~PhysicsManager() {
    Logger::info("Destroying PhysicsManager");
    for (int32_t i = 0; i < _PhysicsBodyCount; ++i) {
        delete _PhysicsBodies[i];
    }
}

PhysicsBody* PhysicsManager::loadBody(Location &location, int32_t _width, int32_t height) {
    PhysicsBody* body = new PhysicsBody(&location, _width, height);
    _PhysicsBodies[_PhysicsBodyCount++] = body;
    return body;
}

void PhysicsManager::update() {
    float timeStamp = _TimeManager.elapsed();
    for (int32_t j = 0; j < _PhysicsBodyCount; ++ j) {
        PhysicsBody* body = _PhysicsBodies[j];
        body->location->x += (timeStamp * body->velocityX);
        body->location->y += (timeStamp * body->velocityY);
    }
}