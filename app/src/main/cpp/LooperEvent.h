//
// Lifecycle observer class
//

#ifndef ANDROIDNATIVEBLUSTER_LOOPER_H
#define ANDROIDNATIVEBLUSTER_LOOPER_H

#include "../../../../../../Library/Android/sdk/ndk-bundle/sources/android/native_app_glue/android_native_app_glue.h"
#include "ActivityHandler.h"
class EventLifecycle {
public:
    EventLifecycle(android_app *, ActivityHandler&);
    void run();

private:
    android_app * _pApplication;
    bool _Enabled;
    bool _Quit;
    ActivityHandler& _ActivityHandler;

private:
    void activate();
    void deactivate();
    void processAppEvent(int32_t);
    static void callback_appEvent(android_app *, int32_t);
};

#endif //ANDROIDNATIVEBLUSTER_LOOPER_H
