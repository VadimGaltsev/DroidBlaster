//
// Created by vadik on 12.06.2018.
//

#ifndef ANDROIDNATIVEBLUSTER_SHIP_H
#define ANDROIDNATIVEBLUSTER_SHIP_H

#include "../engine/Sprite.h"
#include "../managers/GraphicsManager.h"
#include "../managers/PhysicsManager.h"
class Ship {
public:
    Ship(android_app * androidApp, GraphicsManager& graphicsManager);
    void registerShip(Sprite * graphicsElement, b2Body*);
    void initialize();
    void update();
    bool isDestroy() { return isDestroyed; }
private:
    GraphicsManager& _GraphicManager;
    Sprite* _GraphicElement;
    bool isDestroyed;
    b2Body* Body;
    int32_t lives;
};


#endif //ANDROIDNATIVEBLUSTER_SHIP_H
