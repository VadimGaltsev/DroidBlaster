//
// Created by vadik on 06.07.2018.
//

#ifndef ANDROIDNATIVEBLUSTER_ASTEROID_H
#define ANDROIDNATIVEBLUSTER_ASTEROID_H

#include "../../../../../../../Library/Android/sdk/ndk-bundle/sources/android/native_app_glue/android_native_app_glue.h"
#include "../managers/TimeManager.h"
#include "../Types.h"
#include "../managers/PhysicsManager.h"
#include "../managers/GraphicsManager.h"
class Asteroid {
public:
    Asteroid(android_app * app, TimeManager& _TimeManager, GraphicsManager& graphicsManager, PhysicsManager& physicsManager);
    void registerAsteroid(Location& location, int32_t sizeX, int32_t sizeY);
    void initialize();
    void update();

private:
    void spawn(PhysicsBody* _body);
    TimeManager& _TimeManager;
    GraphicsManager& _GraphicsManager;
    PhysicsManager& _PhysicsManager;

    PhysicsBody* _Bodies[1024];
    int32_t _BodiesCount;
    float _MinBound;
    float _LowerBound;
    float _UpperBound;
    float _LeftBound; float _RightBound;
};
#endif //ANDROIDNATIVEBLUSTER_ASTEROID_H
