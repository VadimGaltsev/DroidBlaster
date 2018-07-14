//
// Created by Vadim on 11.07.2018.
//

#ifndef DROIDBLASTER_RESOURCE_H
#define DROIDBLASTER_RESOURCE_H

#include <android/asset_manager.h>
#include "../Types.h"
#include "../../../../../../../Library/Android/sdk/ndk-bundle/sources/android/native_app_glue/android_native_app_glue.h"

class Resource {
public:
    Resource(android_app* app, const char * path);
    const char* getPath() { return _Path; }
    status open();
    void close();
    bool operator == (const Resource& resource);
    status read(void * pBuffer, size_t _Count);

private:
    const char * _Path;
    AAssetManager* assetManager;
    AAsset* aAsset;

};


#endif //DROIDBLASTER_RESOURCE_H
