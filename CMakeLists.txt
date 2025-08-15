cmake_minimum_required(VERSION 3.31)
project(
    qhttpserver
    LANGUAGES C CXX
    VERSION 1.1.0
    DESCRIPTION "QT HTTP server"
)

set(libname "qhttpserver")

###############
# ASI default settings
list(APPEND CMAKE_MODULE_PATH "$ENV{ASV_CMAKE}")
include(asv_cmake_defaults)
asv_cmake_defaults()
###############

find_package(Qt5 CONFIG REQUIRED
    PATHS ${ASV_PLAT_PORTS}/qt5 NO_DEFAULT_PATH
    COMPONENTS
        Network
)
asv_print_package_properties(
    Qt5::Network
)


# -----------------------
# Public header list
# -----------------------
set(public_headers
    src/qhttpconnection.h
    src/qhttprequest.h
    src/qhttpresponse.h
    src/qhttpserver.h
    src/qhttpserverfwd.h
    src/qhttpserver_api.h
)

# Define library
add_library(${libname} SHARED)

# Use file sets for public headers
target_sources(${libname} PUBLIC
    FILE_SET HEADERS
    BASE_DIRS src
    FILES ${public_headers}
)

# Include paths
target_include_directories(${libname}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        http-parser
)

if(MSVC)
    # All compile-time flags in one call
    target_compile_options(${libname} PRIVATE
        # Common flags for all configurations
        "-Gw"                        # Global data optimization
        "-Gy"                        # Function-level linking
        "-Zc:inline"                 # Remove unreferenced inline functions
        "-Gd"                        # Use __cdecl (smaller than __stdcall)

        # Debug-specific flags
        $<$<CONFIG:Debug>:-Od>       # Disable optimization for debugging

        # Release-specific flags
        $<$<CONFIG:Release>:-O2>     # Optimize for speed
        $<$<CONFIG:Release>:-Ob2>    # Inline expansion
        $<$<CONFIG:Release>:-GL>     # Whole program optimization
        $<$<CONFIG:Release>:-GF>     # String pooling
        $<$<CONFIG:Release>:-GS->    # Disable security checks
    )

    # Link-time flags
    set_target_properties(${libname} PROPERTIES
        LINK_FLAGS_RELEASE "-LTCG:incremental -OPT:REF -OPT:ICF=10 -OPT:LBR"
        LINK_FLAGS_RELWITHDEBINFO "-LTCG:incremental -OPT:REF -OPT:ICF=5"
        LINK_FLAGS_DEBUG "-OPT:NOREF -OPT:NOICF"  # Keep debug symbols intact
    )
endif()

target_link_libraries(${libname} PRIVATE Qt5::Network)

# Source files
target_sources(${libname} PRIVATE
    src/qhttpconnection.cpp
    src/qhttprequest.cpp
    src/qhttpresponse.cpp
    src/qhttpserver.cpp
    http-parser/http_parser.c
)

# Platform-specific sources
if(UNIX)
    target_sources(${libname} PRIVATE
        http-parser/contrib/url_parser.c
        http-parser/http_parser.h
    )
    set_target_properties(${libname} PROPERTIES LINK_FLAGS "-Wl,--no-undefined")
endif()

set_target_properties(${libname} PROPERTIES
    AUTOMOC ON
    OUTPUT_NAME "Qt5HttpServer")

# Install library and headers
install(TARGETS ${libname}
    EXPORT ${PROJECT_NAME}Targets
    FILE_SET HEADERS DESTINATION include/qhttpserver
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# Export targets
install(EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION lib/cmake/${PROJECT_NAME}
)

# Generate and install CMake config files
include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

# Use configure_package_config_file to properly expand @PACKAGE_INIT@
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}Config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION lib/cmake/${PROJECT_NAME}
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION lib/cmake/${PROJECT_NAME}
)

# CPack packaging
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION "QT HTTP server")
set(CPACK_PACKAGE_VENDOR "xAI")
set(CPACK_GENERATOR "ZIP;TGZ")
include(CPack)
