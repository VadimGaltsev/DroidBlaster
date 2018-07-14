//
// Created by Vadim on 11.07.2018.
//

#include "Resource.h"
#include <sys/stat.h>
Resource::Resource(android_app *app, const char *path) : _Path(path), assetManager(app->activity->assetManager),
                                                         aAsset(nullptr) {}

status Resource::open() {
    aAsset = AAssetManager_open(assetManager, _Path, AASSET_MODE_UNKNOWN);
    return (aAsset != nullptr) ? STATUS_OK : STATUS_KO;
}

status Resource::read(void *pBuffer, size_t _Count) {
    int32_t readCount = AAsset_read(aAsset, pBuffer, _Count);
    return (readCount == _Count) ? STATUS_OK : STATUS_KO;
}

void Resource::close() {
    if (aAsset != NULL) {
        AAsset_close(aAsset);
        aAsset = nullptr;
    }
}

bool Resource::operator==(const Resource &resource) {
    return !strcmp(_Path, resource._Path);
}