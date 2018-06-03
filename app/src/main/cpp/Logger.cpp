//
// Logger implementation class
//
#include "Logger.h"
#include <stdarg.h>
#include <android/log.h>
#if !defined(LOGGER_IMPL)
#define LOGGER_IMPL
#define MESSAGE_INFO(pMessage, vaArgs) __android_log_vprint(ANDROID_LOG_INFO, PACKT, pMessage, vaArgs); \
    __android_log_print(ANDROID_LOG_INFO, PACKT, "\n");
#define MESSAGE_DEBUG(pMessage, vaArgs) __android_log_vprint(ANDROID_LOG_DEBUG, PACKT, pMessage, vaArgs); \
    __android_log_print(ANDROID_LOG_DEBUG, PACKT, "\n");
#define MESSAGE_ERROR(pMessage, vaArgs) __android_log_vprint(ANDROID_LOG_ERROR, PACKT, pMessage, vaArgs); \
    __android_log_print(ANDROID_LOG_ERROR, PACKT, "\n");
#define MESSAGE_WARN(pMessage, vaArgs) __android_log_vprint(ANDROID_LOG_WARN, PACKT, pMessage, vaArgs); \
    __android_log_print(ANDROID_LOG_WARN, PACKT, "\n");
#define LOGGER_METHOD(message, type) \
    va_list vaArgs; \
    va_start(vaArgs, message); \
    type(message, vaArgs) \
    va_end(vaArgs);
#endif

void Logger::info(const char *_pMessage, ...) {
    LOGGER_METHOD(_pMessage, MESSAGE_INFO);

}
void Logger::debug(const char *_pMessage, ...) {
    LOGGER_METHOD(_pMessage, MESSAGE_DEBUG);
}
void Logger::error(const char *_pMessage, ...) {
    LOGGER_METHOD(_pMessage, MESSAGE_ERROR);
}
void Logger::warn(const char *_pMessage, ...) {
    LOGGER_METHOD(_pMessage, MESSAGE_WARN);
}