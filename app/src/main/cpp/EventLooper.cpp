//
// implementing android app lifecycle in logger
//

#include "LooperEvent.h"
#include "helpers/Logger.h"
#define STARTING "Starting Event Loop"
#define EXIT "Exiting Event Loop"

EventLifecycle::EventLifecycle
        (android_app *_app, ActivityHandler & _activityHandler) : _pApplication(_app),
                                                                                        _ActivityHandler(_activityHandler),
                                                                                        _Quit(false), _Enabled(false) {
    _pApplication->userData = this;
    _pApplication->onAppCmd = callback_appEvent;
}

void EventLifecycle::run() {
    int32_t result, events;
    android_poll_source *source;
    //lock current code
   // app_dummy(); //it is not necessary

    Logger::info(STARTING);
    while (true) {
        // loop and get events
        while ((result = ALooper_pollAll(_Enabled ? 0 : -1, NULL, &events, (void**) &source)) >= 0) {
            //events for get
            if (source != NULL) {
                source->process(_pApplication, source);
            }
            //if app closed
            if (_pApplication->destroyRequested) {
                Logger::info(EXIT);
                return;
            }
        }

        if (_Enabled && !_Quit) {
            if (_ActivityHandler.onStep() != STATUS_OK) {
                _Quit = true;
                ANativeActivity_finish(_pApplication->activity);
            }
        }
    }
}

void EventLifecycle::activate() {
    //activate visual component only if window enabled
    if (!_Enabled && _pApplication->window != NULL) {
        _Quit = false; _Enabled = true;
        if (_ActivityHandler.onActive() != STATUS_OK) {
            goto ERROR;
        }
    }
    return;
    ERROR: {
    _Quit = true;
    deactivate();
    ANativeActivity_finish(_pApplication->activity);
}
}

void EventLifecycle::deactivate() {
    if (_Enabled) {
        _ActivityHandler.onDeactivate();
        _Enabled = false;
    }
}

void EventLifecycle::callback_appEvent(android_app * _app, int32_t _command) {
    //get ptr for our class from structure
    EventLifecycle& _eventLoop = *(static_cast<EventLifecycle *>(_app->userData));
    _eventLoop.processAppEvent(_command);
}

void EventLifecycle::processAppEvent(int32_t _command) {
    switch (_command) {
        case APP_CMD_CONFIG_CHANGED:
            _ActivityHandler.onConfigurationChanged();
            break;
        case APP_CMD_INIT_WINDOW:
            _ActivityHandler.onCreateWindow();
            break;
        case APP_CMD_DESTROY:
            _ActivityHandler.onDestroy();
            break;
        case APP_CMD_GAINED_FOCUS:
            activate();
            _ActivityHandler.onGainFocus();
            break;
        case APP_CMD_LOST_FOCUS:
            _ActivityHandler.onLostFocus();
            deactivate();
            break;
        case APP_CMD_LOW_MEMORY:
            _ActivityHandler.onLowMemory();
            break;
        case APP_CMD_PAUSE:
            _ActivityHandler.onPause();
            deactivate();
            break;
        case APP_CMD_RESUME:
            _ActivityHandler.onResume();
            break;
        case APP_CMD_SAVE_STATE:
            _ActivityHandler.onSaveInstanceState(&_pApplication->savedState,
                                                 &_pApplication->savedStateSize);
            break;
        case APP_CMD_START:
            _ActivityHandler.onStart();
            break;
        case APP_CMD_STOP:
            _ActivityHandler.onStop();
            break;
        case APP_CMD_TERM_WINDOW:
            _ActivityHandler.onDestroyWindow();
            deactivate();
            break;
        default:
            break;
    }
}