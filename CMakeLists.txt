# evan. Copyright (C) 2017.


#
# Project Properties
#
cmake_minimum_required (VERSION 2.8)
project (libev_example)
set (APPLICATION_NAME "libev_example")
set (APPLICATION_CODENAME "${PROJECT_NAME}")
set (APPLICATION_COPYRIGHT_YEARS "2017")
set (APPLICATION_VERSION_MAJOR 0)
set (APPLICATION_VERSION_MINOR 1)
set (APPLICATION_VERSION_PATCH 0)
set (APPLICATION_VERSION_TYPE SNAPSHOT)
set (APPLICATION_VERSION_STRING "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}.${APPLICATION_VERSION_PATCH}-${APPLICATION_VERSION_TYPE}")
#set (APPLICATION_VENDOR_ID "com.yourcompany")
#set (APPLICATION_VENDOR_NAME "Your Company")
#set (APPLICATION_VENDOR_URL "yourcompany.com")
#set (APPLICATION_ID "${APPLICATION_VENDOR_ID}.${PROJECT_NAME}")

set (APPLICATION_GIT_COMMIT "1")
find_package(Git)
if(GIT_FOUND)
  message("git found: ${GIT_EXECUTABLE}")
  execute_process(
    COMMAND ${GIT_EXECUTABLE} log -n1 --pretty=%H
            OUTPUT_VARIABLE GIT_COMMIT
            OUTPUT_STRIP_TRAILING_WHITESPACE)
    set (APPLICATION_GIT_COMMIT "${GIT_COMMIT}")
endif()


# options
# Options. Turn on with 'cmake -Dmyvarname=ON'.
option(test "Build all tests." ON)
option (debug_mode "Debug 开发模式" ON)

#
# Debugging Options
#

set (CMAKE_VERBOSE_MAKEFILE 0) # Use 1 for debugging, 0 for release
if (debug_mode)
    set(CMAKE_BUILD_TYPE Debug)
else()
    set(CMAKE_BUILD_TYPE Release)
endif()

# Use Release if no configuration specified
#if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
#   set(CMAKE_BUILD_TYPE Release)
#endif(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)

if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -Wall -Wno-unused-variable ")
else()
    # Release mode
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2 ")
endif()

message("Build Type: ${CMAKE_BUILD_TYPE} ${CMAKE_CXX_FLAGS}")


#
# Project Output Paths
#
set (MAINFOLDER ${PROJECT_SOURCE_DIR})
set (EXECUTABLE_OUTPUT_PATH "${MAINFOLDER}/bin")
set (LIBRARY_OUTPUT_PATH "${MAINFOLDER}/lib")
set (GTEST_DIR "${MAINFOLDER}/thirdparty/gtest")
set (LIBEV_DIR "${MAINFOLDER}/thirdparty/libev")

#
# Project Search Paths
#
list (APPEND CMAKE_PREFIX_PATH "${MAINFOLDER}")
include_directories("${MAINFOLDER}/include")
include_directories("${LIBEV_DIR}/include")
link_directories("${LIBEV_DIR}/lib")

# RPATH {
set(CMAKE_MACOSX_RPATH 0)
SET(MACOSX_RPATH 0)

set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib" )
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
# RPATH }

configure_file ( "${PROJECT_SOURCE_DIR}/include/stdcfg.hpp.in" "${PROJECT_SOURCE_DIR}/include/stdcfg.hpp" )

#
# Add Build Targets
#
add_subdirectory(src)

if(test)
    enable_testing()
    include_directories("${MAINFOLDER}/test/include")
    add_subdirectory(test)

endif()


#
# Add Install Targets
#

set (CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/dist")

if (EXISTS "${MAINFOLDER}/include/${PROJECT_NAME}" AND IS_DIRECTORY "${MAINFOLDER}/include/${PROJECT_NAME}")
    install(DIRECTORY "${MAINFOLDER}/include/${PROJECT_NAME}" DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
endif (EXISTS "${MAINFOLDER}/include/${PROJECT_NAME}" AND IS_DIRECTORY "${MAINFOLDER}/include/${PROJECT_NAME}")

if (EXISTS "${MAINFOLDER}/etc" AND IS_DIRECTORY "${MAINFOLDER}/etc")
    install(DIRECTORY "${MAINFOLDER}/etc" DESTINATION ${CMAKE_INSTALL_PREFIX}/etc)
endif (EXISTS "${MAINFOLDER}/etc" AND IS_DIRECTORY "${MAINFOLDER}/etc")



