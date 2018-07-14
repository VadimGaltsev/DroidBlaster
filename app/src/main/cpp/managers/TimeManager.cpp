//
// Created by vadik on 06.07.2018.
//

#include "TimeManager.h"
#include "../helpers/Logger.h"
#include <cstdlib>
#include <time.h>
TimeManager::TimeManager() :
_FirstTime(0.0f),
_LastTime(0.0f),
_Elapsed(0.0f),
_ElapsedTotal(0.0f) {
    srand(static_cast<unsigned int>(time(NULL)));
}

void TimeManager::reset() {
    Logger::info("Resetting TimeManager.");
    _Elapsed = 0.0f;
    _FirstTime = now();
    _LastTime = _FirstTime;
}

void TimeManager::update() {
    double currentTime = now();
    _Elapsed = static_cast<float>(currentTime - _LastTime);
    _ElapsedTotal = static_cast<float>(currentTime - _FirstTime);
    _LastTime = currentTime;
}

double TimeManager::now() {
    timespec timeVal;
    clock_gettime(CLOCK_MONOTONIC, &timeVal);
    return timeVal.tv_sec + (timeVal.tv_nsec * 1.0e-9);
}

