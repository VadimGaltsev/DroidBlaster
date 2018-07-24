//
// Created by Vadim on 18.07.2018.
//

#include "MoveableBody.h"
static const float MOVE_SPEED = 10.0f / PHYSICS_SCALE;


MoveableBody::MoveableBody(android_app * app, InputManager & inputManager, PhysicsManager & physicsManager) :
        _InputManager(inputManager),
        _PhysicsManager(physicsManager),
        Body(nullptr), Target(nullptr) {}

void MoveableBody::initialize() {
    Body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
}

b2Body* MoveableBody::registerMoveableBody(Location & loc, int32_t x, int32_t y) {
    Body = _PhysicsManager.loadBody(loc, 0x2, 0x1, x, y, 0.0f);
    Target = _PhysicsManager.loadTarget(Body);
    _InputManager.setRefPoint(&loc);
    return Body;
}

void MoveableBody::update() {
    b2Vec2 target = Body->GetPosition() + b2Vec2(
            _InputManager.getDirectionX() * MOVE_SPEED,
            _InputManager.getDirectionY() * MOVE_SPEED);
    Target->SetTarget(target);
}