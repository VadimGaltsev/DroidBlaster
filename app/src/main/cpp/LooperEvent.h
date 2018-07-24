//
// Lifecycle observer class
//

#ifndef ANDROIDNATIVEBLUSTER_LOOPER_H
#define ANDROIDNATIVEBLUSTER_LOOPER_H

#include "../../../../../../Library/Android/sdk/ndk-bundle/sources/android/native_app_glue/android_native_app_glue.h"
#include "handlers/ActivityHandler.h"
#include "handlers/InputHandler.h"

class EventLifecycle {
public:
    EventLifecycle(android_app *, ActivityHandler&, InputHandler&);
    void run();

private:
    android_app * _pApplication;
    bool _Enabled;
    bool _Quit;
    ActivityHandler& _ActivityHandler;
    InputHandler& _InputHandler;
    ASensorManager* _SensorManager;
    ASensorEventQueue* _SensorEventQueue;
    android_poll_source Sensor_PollSource;
    const ASensor* _Accelerometer;
private:
    void activate();
    void deactivate();
    void processAppEvent(int32_t);
    void processEventSensor();
    void activateAccelerometer();
    void deactivateAccelerometer();
    static void callback_appEvent(android_app *, int32_t);
    int32_t processInputEvent(AInputEvent*);
    static int32_t callbackInput(android_app*, AInputEvent*);
    static void callbackSensor(android_app*, android_poll_source*);

};

#endif //ANDROIDNATIVEBLUSTER_LOOPER_H
