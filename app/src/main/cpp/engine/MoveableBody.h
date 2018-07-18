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
    PhysicsBody* registerMoveableBody(Location&, int32_t, int32_t);
    void initialize();
    void update();

private:
    PhysicsManager& _PhysicsManager;
    InputManager& _InputManager;
    PhysicsBody* Body;

};


#endif //DROIDBLASTER_MOVEABLEBODY_H
