CMAKE_MINIMUM_REQUIRED ( VERSION 3.0 )
project(salt)
SET ( CMAKE_CXX_COMPILER "g++" )
SET ( CMAKE_CXX_STANDARD 11)

if (APPLE)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")
endif()
if (UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
endif()

#SET ( CMAKE_BUILD_TYPE Debug )
SET ( CMAKE_BUILD_TYPE Release )

# release - statically linking boost
# set(Boost_USE_STATIC_LIBS ON)

if (APPLE)
    find_package(Boost COMPONENTS system filesystem serialization REQUIRED)
endif()
if (UNIX)
    find_package(Boost COMPONENTS system filesystem thread serialization REQUIRED)
endif()


if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    # link_directories({Boost_FILESYSTEM_LIBRARY})
    # link_directories(/usr/local/Cellar/boost/1.67.0_1/lib)
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARY_DIRS: ${Boost_FILESYSTEM_LIBRARY}")
    #    message(STATUS "Boost_VERSION: ${Boost_VERSION}")
endif()

find_library(SALTLIB NAMES libsaltstatic.a HINTS ../lib/ REQUIRED)

INCLUDE_DIRECTORIES(./ ../ ../include ../lib)

if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG=1)
endif()

SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin")
SET (PROJECT_ROOT_DIR ${CMAKE_SOURCE_DIR}/..)

###########
# LIBSALT #
###########

if (${CMAKE_VERSION} VERSION_GREATER 3.13.0)
    # UseSWIG generates standard target names
    cmake_policy(SET CMP0078 NEW)
    cmake_policy(SET CMP0079 NEW)
endif()
if (${CMAKE_VERSION} VERSION_GREATER 3.14.0)
    # pass module name to swig compiler
    cmake_policy(SET CMP0086 NEW)
endif()

SET (libsalt_SRC
        libsalt/LibsaltDefs.h
        libsalt/Simulation.cpp
        libsalt/Simulation.h
        libsalt/Node.cpp
        libsalt/Node.h
        libsalt/Link.cpp
        libsalt/Link.h
        libsalt/Lane.cpp
        libsalt/Lane.h
        libsalt/Cell.cpp
        libsalt/Cell.h
        libsalt/Connection.cpp
        libsalt/Connection.h
        libsalt/TrafficSignal.cpp
        libsalt/TrafficSignal.h
        libsalt/Vehicle.cpp
        libsalt/Vehicle.h
        libsalt/VisMsg.h
        libsalt/VisClient.cpp
        libsalt/VisClient.hpp
)

add_library(libsaltstatic STATIC ${libsalt_SRC})
set_property(TARGET libsaltstatic PROPERTY POSITION_INDEPENDENT_CODE ON)
set_property(TARGET libsaltstatic PROPERTY FOLDER "libsalt")

#set(saltlibs libsaltstatic ${PYTHON_LIBRARIES})

#include_directories(./ ../ ../include ../include/Controller ../include/Object ../lib)

set(ENABLE_PYTHON_BINDINGS ON CACHE BOOL "Build Python Bindings")
set(DEFAULT_LIBSALT_PYTHON ON CACHE BOOL "Include Python Bindings in the ALL target")

find_package(SWIG 3.0)
if (SWIG_FOUND)
    set(ENABLED_FEATURES "${ENABLED_FEATURES} SWIG")
endif ()

#set(Python_ADDITIONAL_VERSIONS 3.7) # it may help in finding the correct python for libsumo
find_package(PythonInterp REQUIRED)
#message(STATUS "Found Python: " ${PYTHON_EXECUTABLE})
execute_process(COMMAND ${PYTHON_EXECUTABLE} -c "import setuptools" RESULT_VARIABLE SETUPTOOLS_MISSING)

if (SWIG_FOUND)
    include(${SWIG_USE_FILE})
    set_source_files_properties(libsalt/libsalt.i PROPERTIES CPLUSPLUS ON)
    file(GLOB LIBSALT_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/libsalt/*.h")
    find_package(PythonLibs)
    if (ENABLE_PYTHON_BINDINGS)
        if (MSVC)
            # recheck that the platform of the generator and python matches
            execute_process(COMMAND ${PYTHON_EXECUTABLE} -c "import sys; print(sys.maxsize > 2**32)"
                    OUTPUT_VARIABLE IS_PYTHON64 OUTPUT_STRIP_TRAILING_WHITESPACE)
            if (${CMAKE_MODULE_LINKER_FLAGS} STREQUAL "/machine:x64")
                if (${IS_PYTHON64} STREQUAL "False")
                    message(STATUS "Did not find Python 64 bit. Please set PYTHON_EXECUTABLE, PYTHON_INCLUDE_DIR and PYTHON_LIBRARY manually.")
                    set(PYTHONLIBS_FOUND OFF)
                endif()
            else()
                if (${IS_PYTHON64} STREQUAL "True")
                    message(STATUS "Did not find Python 32 bit. Please set PYTHON_EXECUTABLE, PYTHON_INCLUDE_DIR and PYTHON_LIBRARY manually.")
                    set(PYTHONLIBS_FOUND OFF)
                endif()
            endif()
        endif()

        if (PYTHONLIBS_FOUND)
            if (PYTHON_DEBUG_LIBRARY OR NOT "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
                include_directories(${PYTHON_INCLUDE_PATH})

                # set(CMAKE_SWIG_FLAGS "")
                set(CMAKE_SWIG_FLAGS "-keyword") # is necessary for using keyword args, but generates lots of warnings
                # other options which should be evaluated: -builtin, -modern, -extranative
                set(CMAKE_SWIG_OUTDIR ${PROJECT_ROOT_DIR}/tools/libsalt)
                set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_ROOT_DIR}/tools/libsalt)
                set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PROJECT_ROOT_DIR}/tools/libsalt)
                set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PROJECT_ROOT_DIR}/tools/libsalt)
                set(SWIG_MODULE_libsalt_EXTRA_DEPS ${LIBSALT_HEADERS})

                if (${CMAKE_VERSION} VERSION_GREATER 3.8.0)
                    SWIG_ADD_LIBRARY(libsalt LANGUAGE python SOURCES libsalt/libsalt.i)
                else()
                    SWIG_ADD_MODULE(libsalt python libsalt/libsalt.i)
                endif()

                if (NOT DEFAULT_LIBSALT_PYTHON)
                    # needed to selectively build the python module on the build server
                    set_property(TARGET ${SWIG_MODULE_libsalt_REAL_NAME} PROPERTY EXCLUDE_FROM_ALL TRUE)
                endif()

                if (MSVC OR APPLE)
                    # disable python module for the debug build because virtually no one has a python debug dll to link against
                    set_property(TARGET ${SWIG_MODULE_libsalt_REAL_NAME} PROPERTY EXCLUDE_FROM_DEFAULT_BUILD_DEBUG TRUE)
                    set_property(TARGET ${SWIG_MODULE_libsalt_REAL_NAME} PROPERTY FOLDER "libsalt")
                    swig_link_libraries(libsalt ${SALTLIB} libsaltstatic ${PYTHON_LIBRARIES} ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_SERIALIZATION_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})
                else()
                    swig_link_libraries(libsalt -Wl,--whole-archive ${SALTLIB} libsaltstatic  -Wl,--no-whole-archive ${PYTHON_LIBRARIES} ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_SERIALIZATION_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})
                endif()

                if (NOT ${SETUPTOOLS_MISSING})
                    install(
                            CODE "execute_process(COMMAND ${PYTHON_EXECUTABLE} ${PROJECT_ROOT_DIR}/tools/build/setup-libsalt.py clean --all install --root=\$ENV{DESTDIR}/ --prefix=${CMAKE_INSTALL_PREFIX} --optimize=1)"
                            COMPONENT pylibsalt)
                endif()
            endif()
        else()
            message(WARNING "ENABLE_PYTHON_BINDINGS is set but Python libraries were not found.")
        endif()
    endif()

endif()

