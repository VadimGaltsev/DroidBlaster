//
// Created by Vadim on 18.07.2018.
//

#include "MoveableBody.h"


MoveableBody::MoveableBody(android_app * app, InputManager & inputManager, PhysicsManager & physicsManager) :
        _InputManager(inputManager),
        _PhysicsManager(physicsManager),
        Body(nullptr){}

void MoveableBody::initialize() {
    Body->velocityX = 0.0f;
    Body->velocityY = 0.0f;
}

PhysicsBody* MoveableBody::registerMoveableBody(Location & loc, int32_t x, int32_t y) {
    Body = _PhysicsManager.loadBody(loc, x, y);
    _InputManager.setRefPoint(&loc);
    return Body;
}

void MoveableBody::update() {
    static const float MOVE_SPEED = 320.0f;
    Body->velocityX = _InputManager.getDirectionX() * MOVE_SPEED;
    Body->velocityY = _InputManager.getDirectionY() * MOVE_SPEED;

}