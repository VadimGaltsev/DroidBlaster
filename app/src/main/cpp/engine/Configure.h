//
// Created by Vadim on 23.07.2018.
//

#ifndef DROIDBLASTER_CONFIGURE_H
#define DROIDBLASTER_CONFIGURE_H

#include "../Types.h"
#include "../../../../../../../Library/Android/sdk/ndk-bundle/sources/android/native_app_glue/android_native_app_glue.h"
#include <jni.h>

typedef int32_t screen_rotation;

const screen_rotation  ROTATION_0 = 0;
const screen_rotation ROTATION_90 = 1;
const screen_rotation ROTATION_180 = 2;
const screen_rotation ROTATION_270 = 3;

class Configure {
public:
    Configure(android_app*);
    screen_rotation getRotation() { return Rotation; }

private:
    void findRotation(JNIEnv*);
    android_app* _App;
    screen_rotation Rotation;
};


#endif //DROIDBLASTER_CONFIGURE_H
