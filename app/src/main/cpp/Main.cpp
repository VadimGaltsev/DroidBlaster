//
// Enter point in android app
//

#include "LooperEvent.h"
#include "DroidBlaster.h"
#include "Logger.h"

void android_main(android_app * _pApplication) {
    DroidBlaster(_pApplication).run();
}


