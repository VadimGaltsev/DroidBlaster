//
// Logger header for using android native log API
//

#ifndef ANDROIDNATIVEBLUSTER_LOGGER_H
#define ANDROIDNATIVEBLUSTER_LOGGER_H
#define PACKT "PACKT"
extern "C"
class Logger {
public:
    static void error(const char * _pMessage, ...);
    static void warn(const char * _pMessage, ...);
    static void info(const char * _pMessage, ...);
    static void debug(const char * _pMessage, ...);
};

#ifndef NDEBUG
    #define pack_log_debug(...) Log::debug(__VA_ARGS__)
#else
    #define pack_log_debug(...)
#endif

#endif
