//
// class for activityHandler implementation
//

#ifndef ANDROIDNATIVEBLUSTER_DROIDBLASTER_H
#define ANDROIDNATIVEBLUSTER_DROIDBLASTER_H

#include "ActivityHandler.h"
#include "LooperEvent.h"
#include "Types.h"
#include "objects/Ship.h"
#include "managers/GraphicsManager.h"
#include "objects/Asteroid.h"
#include "managers/PhysicsManager.h"
#include "managers/TimeManager.h"
#include "helpers/Resource.h"
#include "engine/SpriteBatch.h"
#include "StarField.h"


class DroidBlaster : public ActivityHandler {
public:
    DroidBlaster(android_app *);
    void run();

protected:
    virtual status onActive();
    virtual void onDeactivate();
    virtual status onStep();

    virtual void onStart();
    virtual void onResume();
    virtual void onPause();
    virtual void onStop();
    virtual void onDestroy();

    virtual void onSaveInstanceState(void **, size_t *);
    virtual void onConfigurationChanged();
    virtual void onLowMemory();

    virtual void onCreateWindow();
    virtual void onDestroyWindow();
    virtual void onGainFocus();
    virtual void onLostFocus();
private:
    EventLifecycle _eventLooper;
    GraphicsManager _GraphicManager;
    Ship _Ship;
    StarField _StarField;
    TimeManager _TimeManager;
    PhysicsManager _PhysicsManager;
    Asteroid _Asteroids;
    Resource _AsteroidTexture;
    Resource _ShipTexture;
    Resource _StarTexture;
    SpriteBatch _SpriteBatch;
};
#endif //ANDROIDNATIVEBLUSTER_DROIDBLASTER_H
