//
// Created by vadik on 06.07.2018.
//

#include <cstdlib>
#include "Asteroid.h"
#include "../helpers/Logger.h"
static const float BOUNDS_MARGIN = 128 / PHYSICS_SCALE;
static const float MIN_VELOCITY = 150.0f / PHYSICS_SCALE, VELOCITY_RANGE = 600.0f / PHYSICS_SCALE;

Asteroid::Asteroid(android_app *app, TimeManager &_TimeManager, GraphicsManager &graphicsManager,
                   PhysicsManager &physicsManager) : _TimeManager(_TimeManager),
                    _GraphicsManager(graphicsManager), _PhysicsManager(physicsManager),
                    _Bodies(), _MinBound(0.0f), _UpperBound(0.0f),
                    _LowerBound(0.0f), _LeftBound(0.0f), _RightBound(0.0f) {}

void Asteroid::registerAsteroid(Location &location, int32_t sizeX, int32_t sizeY) {
    _Bodies.push_back(_PhysicsManager.loadBody(location, 0x1, 0x2, sizeX, sizeY, 2.0f));
}

void Asteroid::initialize() {
    _MinBound = _GraphicsManager.getRenderHeight() / PHYSICS_SCALE;
    _UpperBound = _MinBound * 2;
    _LowerBound = -BOUNDS_MARGIN;
    _LeftBound = -BOUNDS_MARGIN;
    _RightBound = ((_GraphicsManager.getRenderWidth() / PHYSICS_SCALE) + BOUNDS_MARGIN);
    std::vector<b2Body*>::iterator body;
    for (body = _Bodies.begin(); body < _Bodies.end(); ++body) {
        spawn(*body);
    }
}

void Asteroid::update() {
    std::vector<b2Body*>::iterator bodyIterator;
    for (bodyIterator = _Bodies.begin(); bodyIterator < _Bodies.end(); ++bodyIterator) {
        b2Body* body = *bodyIterator;
        if ((body->GetPosition().x < _LeftBound)
                || (body->GetPosition().x > _RightBound)
                || (body->GetPosition().y < _LowerBound)
                || (body->GetPosition().y > _UpperBound)) {
            spawn(body);
        }
    }
}

void Asteroid::spawn(b2Body *_body) {
    float velocity = - (RAND(VELOCITY_RANGE) + MIN_VELOCITY);
    float positionX = _LeftBound + RAND(_RightBound - _LeftBound);
    float positionY = _MinBound + RAND(_UpperBound - _MinBound);
    _body->SetTransform(b2Vec2(positionX, positionY), 0.0f);
    _body->SetLinearVelocity(b2Vec2(0.0f, velocity));
}