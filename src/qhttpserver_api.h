#pragma once

#ifdef _WIN32
    // cmake defines <libname>_EXPORTS building shared libraries
    #ifdef qhttpserver_EXPORTS
        #define QHTTPSERVER_API __declspec(dllexport)
    #else
        #define QHTTPSERVER_API __declspec(dllimport)
    #endif // qhttpserver_EXPORTS
#else
    // Linux - gcc/clang
    #define QHTTPSERVER_API __attribute__((visibility("default")))
#endif // WIN32

