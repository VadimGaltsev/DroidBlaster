//
// include header
//

#ifndef ANDROIDNATIVEBLUSTER_TYPES_H
#define ANDROIDNATIVEBLUSTER_TYPES_H

#include <cstdint>
#include <stdio.h>
#include <string.h>

typedef int32_t status;
const status STATUS_OK = 0;
const status STATUS_KO = -1;
const status STATUS_EXIT = -2;

struct Location {
    Location() : x(0.0F), y(0.0) {}
    float x, y;
};

#define RAND(pMAX) (float(pMAX) * float(rand()) / float(RAND_MAX))
#endif //ANDROIDNATIVEBLUSTER_TYPES_H
