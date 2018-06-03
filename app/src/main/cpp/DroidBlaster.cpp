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

DroidBlaster::DroidBlaster(android_app * app) : _eventLooper(app, *this) {
    LOG_INFO(CREATE)
}
void DroidBlaster::run() {
    _eventLooper.run();
}
status DroidBlaster::onActive() {
    LOG_INFO(ON_ACTIVE)
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
    LOG_INFO(STEP_START)
    usleep(300000);
    LOG_INFO(STEP_DONE)
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
