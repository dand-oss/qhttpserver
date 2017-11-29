cmake_minimum_required(VERSION 2.8)
project(qhttpserver  C CXX)

###############
# ASI default settings
list(APPEND CMAKE_MODULE_PATH "$ENV{ASI}/cmake/Modules")
include(asv_cmake_defaults)
asv_cmake_defaults( )
###############

find_package(Qt5 CONFIG REQUIRED COMPONENTS Network)
set(CMAKE_AUTOMOC ON)

list(APPEND srcs
    src/qhttpconnection.cpp
    src/qhttprequest.cpp
    src/qhttpresponse.cpp
    src/qhttpserver.cpp
    http-parser/http_parser.c
    )

if(UNIX)
    list(APPEND srcs
        http-parser/contrib/url_parser.c
	)
endif(UNIX)

list(APPEND hdrs
    src/qhttpconnection.h
    src/qhttprequest.h
    src/qhttpresponse.h
    src/qhttpserver.h
    src/qhttpserverfwd.h
    )

if(UNIX)
    list(APPEND srcs
        http-parser/http_parser.h
	)
endif(UNIX)

include_directories(AFTER src http-parser)

message("moc_sources: ${moc_sources}")

# shared object
set(libname qhttpserver)
add_library(${libname} SHARED ${srcs} ${hdrs})
target_link_libraries(${libname} Qt5::Network)
set_target_properties(${libname} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
if (UNIX)
    # force linker to resolve all deps
    set_target_properties(${libname} PROPERTIES LINK_FLAGS "-Wl,--no-undefined")
    #target_compile_definitions(${libname} QHTTPSERVER_EXPORTS)
endif (UNIX)

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
