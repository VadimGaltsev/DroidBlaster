//
// Created by vadik on 12.06.2018.
//

#ifndef ANDROIDNATIVEBLUSTER_SHIP_H
#define ANDROIDNATIVEBLUSTER_SHIP_H

#include "Sprite.h"
#include "GraphicsManager.h"

class Ship {
public:
    Ship(android_app * androidApp, GraphicsManager& graphicsManager);
    void registerShip(Sprite * graphicsElement);
    void initialize();

private:
    GraphicsManager& _GraphicManager;
    Sprite* _GraphicElement;
};


#endif //ANDROIDNATIVEBLUSTER_SHIP_H
