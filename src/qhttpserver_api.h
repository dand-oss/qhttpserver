#pragma once

#include <QtCore/QtGlobal> // QT_DECL

// cmake shared lib target DEFINE_SYMBOL
#ifdef qhttpserver_EXPORTS
    // when building shared library
    #define QHTTPSERVER_API Q_DECL_EXPORT
#else
    // used outside dll
    #define QHTTPSERVER_API Q_DECL_IMPORT
#endif
