//
// DroidBlaster class realisation
//

#if !defined(DROIDBLASTER)
#define DROIDBLASTER
#define LOG_INFO(...) \
    Logger::info(__VA_ARGS__);
#define CREATE "Creating droidBlaster"
#define ON_START "onStart activity"
#define ON_PAUSE "onPause activty"
#define ON_STOP "onStop activity"
#define ON_RESUME "onResume activity"
#define ON_ACTIVE "Droidblaster activating"
#define ON_DEACTIVE "Droidblaster deactivating"
#define STEP_START "Starting step"
#define STEP_DONE "Stepping done"
#define ON_DESTROY "onDestoy activity"
#define LOW_MEM "onLowMemory in applcation"
#define WIN_CREATE "Window created"
#define WIN_DEST "Window destroyed"
#define CONFIG "onConfiguration changed"
#define SAVE_STATE "onSaveInstanceState come"
#define ON_FOCUS "onGainFocus"
#define LOST_FOCUS "onLostFocus"
#endif

#include <unistd.h>
#include "DroidBlaster.h"
#include "Logger.h"

static const  int32_t  SHIP_SIZE = 64;
static const int32_t  ASTEROIDS_COUNT = 16;
static const int32_t ASTEROID_SIZE = 64;
DroidBlaster::DroidBlaster(android_app * app) : _TimeManager(), _eventLooper(app, *this), _GraphicManager(app),
                                                _PhysicsManager(_TimeManager, _GraphicManager),
                                                _Ship(app, _GraphicManager),
                                                _Asteroids(app, _TimeManager, _GraphicManager, _PhysicsManager) {
    LOG_INFO(CREATE)
    GraphicsElement * shipGraphicElement = _GraphicManager.registerElement(SHIP_SIZE, SHIP_SIZE);
    _Ship.registerShip(shipGraphicElement);
    for (int32_t i = 0; i < ASTEROIDS_COUNT; ++i) {
        GraphicsElement * asteroidGraphics = _GraphicManager.registerElement(ASTEROID_SIZE, ASTEROID_SIZE);
        _Asteroids.registerAsteroid(asteroidGraphics->location, ASTEROID_SIZE, ASTEROID_SIZE);
    }
}
void DroidBlaster::run() {
    _eventLooper.run();
}
status DroidBlaster::onActive() {
    LOG_INFO(ON_ACTIVE)
    if (_GraphicManager.start() != STATUS_OK) {
        return STATUS_KO;
    }
    _Asteroids.initialize();
    _Ship.initialize();
    _TimeManager.reset();
    return STATUS_OK;
}
void DroidBlaster::onDeactivate() {
    LOG_INFO(ON_DEACTIVE)
}
void DroidBlaster::onResume() {
    LOG_INFO(ON_RESUME)
}
void DroidBlaster::onStop() {
    LOG_INFO(ON_STOP)
}
status DroidBlaster::onStep() {
    _TimeManager.update();
    _PhysicsManager.update();
    _Asteroids.update();
    return _GraphicManager.update();
}
void DroidBlaster::onStart() {
    LOG_INFO(ON_START)
}
void DroidBlaster::onDestroyWindow() {
    LOG_INFO(WIN_DEST)
}
void DroidBlaster::onPause() {
    LOG_INFO(ON_PAUSE)
}
void DroidBlaster::onLowMemory() {
    LOG_INFO(LOW_MEM)
}
void DroidBlaster::onLostFocus() {
    LOG_INFO(LOST_FOCUS)
}
void DroidBlaster::onConfigurationChanged() {
    LOG_INFO(CONFIG)
}
void DroidBlaster::onSaveInstanceState(void ** savedState, size_t * size) {
    LOG_INFO(SAVE_STATE, *savedState, *size)
}
void DroidBlaster::onDestroy() {
    LOG_INFO(ON_DESTROY)
}
void DroidBlaster::onGainFocus() {
    LOG_INFO(ON_FOCUS)
}
void DroidBlaster::onCreateWindow() {
    LOG_INFO(WIN_CREATE)
}
