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
#define SHIP_TEXTURE "droidblaster/ship.png"
#define ASTEROID_TEXTURE "droidblaster/asteroid.png"
#define STAR_TEXTURE "droidblaster/star.png"
#endif

#include <unistd.h>
#include <cstdlib>
#include "DroidBlaster.h"
#include "helpers/Logger.h"
#include <dlfcn.h>

static const  int32_t  SHIP_SIZE = 64;
static const int32_t  ASTEROIDS_COUNT = 16;
static const int32_t ASTEROID_SIZE = 64;
static const int32_t SHIP_FRAME_1 = 0;
static const int32_t SHIP_FRAME_COUNT = 8;
static const float SHIP_ANIM_SPEED = 8.0f;
static const int32_t ASTEROID_FRAME_1 = 0;
static const int32_t ASTEROID_FRAME_COUNT = 16;
static const float  ASTEROID_MIN_ANIM_SPEED = 8.0f;
static const float ASTEROID_ANIM_SPEED_RANGE = 16.0f;
static const int32_t STAR_COUNT = 50;

DroidBlaster::DroidBlaster(android_app * app) : _TimeManager(), _GraphicManager(app), _InputManager(app, _GraphicManager),
                                                _eventLooper(app, *this, _InputManager),
                                                _PhysicsManager(_TimeManager, _GraphicManager),
                                                _Ship(app, _GraphicManager),
                                                _Asteroids(app, _TimeManager, _GraphicManager, _PhysicsManager),
                                                _AsteroidTexture(app, ASTEROID_TEXTURE), _StarTexture(app, STAR_TEXTURE),
                                                _ShipTexture(app, SHIP_TEXTURE), _SpriteBatch(_TimeManager, _GraphicManager),
                                                _StarField(app, _TimeManager, _GraphicManager, STAR_COUNT, _StarTexture),
                                                _MoveableBody(app, _InputManager, _PhysicsManager) {
    LOG_INFO(CREATE)
    Sprite * shipSpriteGraphic = _SpriteBatch.registerSprite(_ShipTexture, SHIP_SIZE, SHIP_SIZE);
    shipSpriteGraphic->setAnimation(SHIP_FRAME_1, SHIP_FRAME_COUNT, SHIP_ANIM_SPEED, true);
    _MoveableBody.registerMoveableBody(shipSpriteGraphic->location, SHIP_SIZE, SHIP_SIZE);
    _Ship.registerShip(shipSpriteGraphic);
    for (int32_t i = 0; i < ASTEROIDS_COUNT; ++i) {
        Sprite * asteroidGraphics = _SpriteBatch.registerSprite(_AsteroidTexture, ASTEROID_SIZE, ASTEROID_SIZE);
        float animSpeed = ASTEROID_MIN_ANIM_SPEED + RAND(ASTEROID_ANIM_SPEED_RANGE);
        asteroidGraphics->setAnimation(ASTEROID_FRAME_1, ASTEROID_FRAME_COUNT, animSpeed, true);
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
    _InputManager.start();
    _GraphicManager.loadTexture(_AsteroidTexture);
    _GraphicManager.loadTexture(_ShipTexture);
    _GraphicManager.loadTexture(_StarTexture);
    _Asteroids.initialize();
    _Ship.initialize();
    _MoveableBody.initialize();
    _TimeManager.reset();
    return STATUS_OK;
}
void DroidBlaster::onDeactivate() {
    LOG_INFO(ON_DEACTIVE)
    _GraphicManager.stop();
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
    _MoveableBody.update();
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
