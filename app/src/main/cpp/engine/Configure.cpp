//
// Created by Vadim on 23.07.2018.
//

#include "Configure.h"
#include "../helpers/Logger.h"

Configure::Configure(android_app * pApp) : _App(pApp), Rotation(0) {
    AConfiguration* configuration = AConfiguration_new();
    if (configuration == nullptr) return;

    int32_t  result;
    char i18NBuffer[] = "__";
    static const char* orientation[] = {
            "Unknown", "Portrait", "Landscape", "Square"
    };
    static const char* screenSize[] = {
            "Unknown", "Small", "Normal", "Large", "X-Large"
    };
    static const char* screenLong[] = {
            "Unknown", "No", "Yes"
    };
    AConfiguration_fromAssetManager(configuration, _App->activity->assetManager);
    result = AConfiguration_getSdkVersion(configuration);
    Logger::info("SDK version : %d", result);
    AConfiguration_getLanguage(configuration, i18NBuffer);
    Logger::info("Language : %s", i18NBuffer);
    AConfiguration_getCountry(configuration, i18NBuffer);
    Logger::info("Country : %s", i18NBuffer);
    result = AConfiguration_getOrientation(configuration);
    Logger::info("Orientation : %s (%d)", orientation[result], result);
    result = AConfiguration_getDensity(configuration);
    Logger::info("Density : %d dpi", result);
    result = AConfiguration_getScreenSize(configuration);
    Logger::info("Screen size : %s (%d)", screenSize[result], result);
    result = AConfiguration_getScreenLong(configuration);
    Logger::info("Long screen size : %s (%d)", screenLong[result], result);
    AConfiguration_delete(configuration);

    JavaVM* javaVM = pApp->activity->vm;
    JavaVMAttachArgs javaVMAttachArgs;
    javaVMAttachArgs.version = JNI_VERSION_1_6;
    javaVMAttachArgs.name = "NativeThread";
    javaVMAttachArgs.group = nullptr;
    JNIEnv* env;
    if (javaVM->AttachCurrentThread(&env, &javaVMAttachArgs) != JNI_OK) {
        Logger::error("Cannot attach java vm to current thread");
        return;
    }
    findRotation(env);
    _App->activity->vm->DetachCurrentThread();
}

void Configure::findRotation(JNIEnv * env) {
    jobject WINDOW_SERVICE, windowManager, display;
    jclass ClassActivity, ClassContext;
    jclass ClassWindowManager, ClassDisplay;
    jmethodID MethodGetSystemService, MethodGetDefaultDisplay, MethodGetRotation;
    jobject activity = _App->activity->clazz;

    ClassActivity = env->GetObjectClass(activity);
    ClassContext = env->FindClass("android/content/Context");
    ClassWindowManager = env->FindClass("android/view/WindowManager");
    ClassDisplay = env->FindClass("android/view/Display");
    MethodGetSystemService = env->GetMethodID(ClassActivity,
                                              "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
    MethodGetDefaultDisplay = env->GetMethodID(ClassWindowManager, "getDefaultDisplay", "()Landroid/view/Display;");
    MethodGetRotation = env->GetMethodID(ClassDisplay, "getRotation", "()I");

    jfieldID  WIN_FIELD_SERVICE = env->GetStaticFieldID(ClassContext, "WINDOW_SERVICE", "Ljava/lang/String;");
    WINDOW_SERVICE = env->GetStaticObjectField(ClassContext, WIN_FIELD_SERVICE);
    windowManager = env->CallObjectMethod(activity, MethodGetSystemService, WINDOW_SERVICE);
    display = env->CallObjectMethod(windowManager, MethodGetDefaultDisplay);
    Rotation = env->CallIntMethod(display, MethodGetRotation);

    env->DeleteLocalRef(ClassActivity); env->DeleteLocalRef(ClassContext);
    env->DeleteLocalRef(ClassWindowManager); env->DeleteLocalRef(ClassDisplay);
}