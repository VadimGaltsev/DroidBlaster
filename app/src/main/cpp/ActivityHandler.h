//
// Class for activity lifecycle information
//

#ifndef ANDROIDNATIVEBLUSTER_ACTIVITYHANDLER_H
#define ANDROIDNATIVEBLUSTER_ACTIVITYHANDLER_H


#include "Types.h"

class ActivityHandler {
public:
    virtual ~ActivityHandler() {};

    virtual status onActive() = 0;
    virtual void onDeactivate() = 0;
    virtual status onStep() = 0;

    virtual void onStart() {};
    virtual void onResume() {};
    virtual void onPause() {};
    virtual void onStop() {};
    virtual void onDestroy() {};

    virtual void onSaveInstanceState(void **, size_t *) {};
    virtual void onConfigurationChanged() {};
    virtual void onLowMemory() {};

    virtual void onCreateWindow() {};
    virtual void onDestroyWindow() {};
    virtual void onGainFocus() {};
    virtual void onLostFocus() {};
};


#endif //ANDROIDNATIVEBLUSTER_ACTIVITYHANDLER_H
