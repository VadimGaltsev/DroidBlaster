//
// Created by vadik on 06.07.2018.
//

#include <cstdlib>
#include "Asteroid.h"
#include "Logger.h"
static const float BOUNDS_MARGIN = 128;
static const float MIN_VELOCITY = 150.0f, VELOCITY_RANGE = 600.0f;

Asteroid::Asteroid(android_app *app, TimeManager &_TimeManager, GraphicsManager &graphicsManager,
                   PhysicsManager &physicsManager) : _TimeManager(_TimeManager),
                    _GraphicsManager(graphicsManager), _PhysicsManager(physicsManager),
                    _Bodies(), _BodiesCount(0), _MinBound(0.0f), _UpperBound(0.0f),
                    _LowerBound(0.0f), _LeftBound(0.0f), _RightBound(0.0f) {}

void Asteroid::registerAsteroid(Location &location, int32_t sizeX, int32_t sizeY) {
    _Bodies[_BodiesCount++] = _PhysicsManager.loadBody(location, sizeX, sizeY);
}

void Asteroid::initialize() {
    _MinBound = _GraphicsManager.getRenderHeight();
    _UpperBound = _MinBound * 2;
    _LowerBound = -BOUNDS_MARGIN;
    _LeftBound = -BOUNDS_MARGIN;
    _RightBound = (_GraphicsManager.getRenderWidth() + BOUNDS_MARGIN);
    for (int32_t i = 0; i < _BodiesCount; ++i) {
        spawn(_Bodies[i]);
    }
}

void Asteroid::update() {
    for (int32_t i = 0; i < _BodiesCount; ++i) {
        PhysicsBody* body = _Bodies[i];
        if (body->location->x < _LeftBound || body->location->x > _RightBound
                || body->location->y < _LowerBound || body->location->y > _UpperBound) {
            spawn(body);
        }
    }
}

void Asteroid::spawn(PhysicsBody *_body) {
    float velocity = - (RAND(VELOCITY_RANGE) + MIN_VELOCITY);
    float positionX = RAND(_GraphicsManager.getRenderWidth());
    float positionY = RAND(_GraphicsManager.getRenderHeight()) + _GraphicsManager.getRenderHeight();

    _body->velocityX = 0.0f;
    _body->velocityY = velocity;
    _body->location->x = positionX;
    _body->location->y = positionY;
}