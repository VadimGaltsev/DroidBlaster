//
// Created by Vadim on 18.07.2018.
//

#ifndef DROIDBLASTER_MOVEABLEBODY_H
#define DROIDBLASTER_MOVEABLEBODY_H


#include "../../../../../../../Library/Android/sdk/ndk-bundle/sources/android/native_app_glue/android_native_app_glue.h"
#include "../managers/InputManager.h"
#include "../managers/PhysicsManager.h"

class MoveableBody {
public:
    MoveableBody(android_app*, InputManager&, PhysicsManager&);
    b2Body* registerMoveableBody(Location&, int32_t, int32_t);
    void initialize();
    void update();

private:
    PhysicsManager& _PhysicsManager;
    InputManager& _InputManager;
    b2Body* Body;
    b2MouseJoint* Target;
};


#endif //DROIDBLASTER_MOVEABLEBODY_H
