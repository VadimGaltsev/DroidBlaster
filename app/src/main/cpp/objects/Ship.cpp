//
// Created by vadik on 12.06.2018.
//

#include "Ship.h"
#include "../helpers/Logger.h"
#include "../Types.h"

static const float INITAL_X = 0.5f;
static const float INITAL_Y = 0.25f;
static const int32_t  DEFAULT_LIVES = 10;
static const int32_t SHIP_DESTROY_FRAME_1 = 8;
static const int32_t SHIP_DESTROY_FRAME_COUNT = 9;
static const float SHIP_DESTROY_ANIM_SPEED = 12.0f;

Ship::Ship(android_app *androidApp, GraphicsManager &graphicsManager) :
        _GraphicManager(graphicsManager),
        _GraphicElement(NULL), Body(NULL),
        isDestroyed(false),
        lives(0) {

}

void Ship::registerShip(Sprite *graphicsElement, b2Body* pBody) {
    _GraphicElement = graphicsElement;
    Body = pBody;
}

void Ship::initialize() {
    isDestroyed = false;
    lives = DEFAULT_LIVES;
    b2Vec2 position (_GraphicManager.getRenderWidth() * INITAL_X / PHYSICS_SCALE,
    _GraphicManager.getRenderHeight() * INITAL_Y / PHYSICS_SCALE);
    Body->SetTransform(position, 0.0f);
    Body->SetActive(true);
}

void Ship::update() {
    if (lives >= 0) {
        if (static_cast<PhysicsCollision*>(Body->GetUserData())->collide) {
            --lives;
            if (lives < 0) {
                Logger::info("Ship was destroyed.");
                _GraphicElement->setAnimation(SHIP_DESTROY_FRAME_1,
                                              SHIP_DESTROY_FRAME_COUNT,
                                              SHIP_DESTROY_ANIM_SPEED, false);
                Body->SetActive(false);
            } else {
                Logger::info("Ship collided.");
            }
        }
    } else {
        if (_GraphicElement->endAnimation()) {
            isDestroyed = true;
        }
    }
}