//
// Created by vadik on 06.07.2018.
//

#ifndef ANDROIDNATIVEBLUSTER_TIMEMANAGER_H
#define ANDROIDNATIVEBLUSTER_TIMEMANAGER_H
#include <ctime>
#include "../Types.h"

class TimeManager {
public:
    TimeManager();
    void reset();
    void update();
    double now();
    float elapsed() { return _Elapsed; }
    float elapsedTotel() { return _ElapsedTotal; }

private:
    double _FirstTime;
    double _LastTime;
    float _Elapsed;
    float _ElapsedTotal;
};
#endif //ANDROIDNATIVEBLUSTER_TIMEMANAGER_H
