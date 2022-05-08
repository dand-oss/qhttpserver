cmake_minimum_required(VERSION 3.30)

set(libname "qhttpserver")

# Define library
add_library(${libname} SHARED)

# Include paths
target_include_directories(${libname}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ../http-parser
)

target_link_libraries(${libname} PRIVATE Qt5::Network)

# Source files
target_sources(${libname} PRIVATE
    qhttpconnection.cpp
    qhttprequest.cpp
    qhttpresponse.cpp
    qhttpserver.cpp
    ../http-parser/http_parser.c
)

# Platform-specific sources
if(UNIX)
    target_sources(${libname} PRIVATE
        ../http-parser/contrib/url_parser.c
        ../http-parser/http_parser.h
    )
    set_target_properties(${libname} PROPERTIES LINK_FLAGS "-Wl,--no-undefined")
else()
    set_target_properties(${libname} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# Automoc for Qt
set_target_properties(${libname} PROPERTIES AUTOMOC ON)

# Declare public headers
set_target_properties(${libname} PROPERTIES
    PUBLIC_HEADER
        "qhttpconnection.h;qhttprequest.h;qhttpresponse.h;qhttpserver.h;qhttpserverfwd.h;qhttpserver_api.h"
)

# Install library and headers
install(TARGETS ${libname}
    EXPORT ${PROJECT_NAME}Targets
    PUBLIC_HEADER DESTINATION include/qhttpserver
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
