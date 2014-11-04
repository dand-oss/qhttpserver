cmake_minimum_required(VERSION 3.30)
project(qhttpserver C CXX)
set(libname "qhttpserver")

###############
# ASI default settings
list(APPEND CMAKE_MODULE_PATH "$ENV{ASV_CMAKE}")
include(asv_cmake_defaults)
asv_cmake_defaults( )
###############

list(APPEND CMAKE_PREFIX_PATH ${AQTDIR})
set(QT_LIBINFIX "ASV")
find_package(Qt5 CONFIG REQUIRED COMPONENTS Network)

add_library(${libname} SHARED)
target_include_directories(${libname} PRIVATE src http-parser)
target_link_libraries(${libname} Qt5::Network)
target_sources(${libname} PRIVATE
    src/qhttpconnection.cpp
    src/qhttprequest.cpp
    src/qhttpresponse.cpp
    src/qhttpserver.cpp
    http-parser/http_parser.c

    src/qhttpconnection.h
    src/qhttprequest.h
    src/qhttpresponse.h
    src/qhttpserver.h
    src/qhttpserverfwd.h
)
set_target_properties(${libname} PROPERTIES AUTOMOC ON)

if(UNIX)
    target_sources(${libname} PRIVATE
        http-parser/contrib/url_parser.c
        http-parser/http_parser.h
    )
    # force linker to resolve all deps
    set_target_properties(${libname} PROPERTIES LINK_FLAGS "-Wl,--no-undefined")
else(UNIX)
    set_target_properties(${libname} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif(UNIX)

# libraries
install(
    TARGETS ${libname}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# includes
install(
    FILES
    src/qhttpconnection.h
    src/qhttprequest.h
    src/qhttpresponse.h
    src/qhttpserverfwd.h
    src/qhttpserver.h
    DESTINATION include/qhttpserver
)
