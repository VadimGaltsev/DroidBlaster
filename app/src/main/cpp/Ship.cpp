//
// Created by vadik on 12.06.2018.
//

#include "Ship.h"
#include "Logger.h"
#include "Types.h"

static const float INITAL_X = 0.5f;
static const float INITAL_Y = 0.25f;

Ship::Ship(android_app *androidApp, GraphicsManager &graphicsManager) : _GraphicManager(graphicsManager), _GraphicElement(NULL) {

}

void Ship::registerShip(Sprite *graphicsElement) {
    _GraphicElement = graphicsElement;
}

void Ship::initialize() {
    _GraphicElement->location.x = INITAL_X * _GraphicManager.getRenderWidth();
    _GraphicElement->location.y = INITAL_Y * _GraphicManager.getRenderHeight();
}