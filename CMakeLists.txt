# ********************************************************************
# CMakeLists for anxi
# file: CMakeLists.txt
# author: hhool
# date: 2024-07-16
# ********************************************************************

cmake_minimum_required(VERSION 3.0)

project(anxi LANGUAGES CXX)

message(STATUS "${PROJECT_NAME} CMakeLists")
# set output directory
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
endif()

# set build tpye
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# set global property for folder
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")

# options
option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(USE_UNICODE "Use unicode" ON)
option(ANXI_BUILD_EXAMPLES "Build anxi examples" ON)
option(ANXI_BUILD_DOC "Build anxi Doc" OFF)
option(ANXI_BUILD_TEST "Build anxi Test" ON)
option(ANXI_BUILD_UNITTEST "Build anxi Unittest" ON)
option(ANXI_BUILD_STLOAD_SIMULATOR "Use STLOAD simulator" ON)

# only build library if add anxi as subdirectory
if(NOT CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    set(ANXI_BUILD_EXAMPLES OFF)
    set(ANXI_BUILD_DOC OFF)
    set(ANXI_BUILD_TEST OFF)
    set(ANXI_BUILD_UNITTEST OFF)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    set(ANXI_BUILD_EXAMPLES OFF)
    set(ANXI_BUILD_DOC OFF)
    set(ANXI_BUILD_TEST OFF)
    set(ANXI_BUILD_UNITTEST OFF)
endif()

# anxi UNICODE define for windows platform
if(WIN32)
    if(USE_UNICODE)
        add_definitions(-DUNICODE -D_UNICODE)
    endif()

    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

message(STATUS "anxi CMake Info")
message(STATUS "=======================================================")
message(STATUS "    Operation System :  ${CMAKE_SYSTEM}")
message(STATUS "    CPU Architecture : ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "    Build Type : ${CMAKE_BUILD_TYPE}")
message(STATUS "    Shared Library  : ${BUILD_SHARED_LIBS}")
message(STATUS "    Build Examples  : ${ANXI_BUILD_EXAMPLES}")
message(STATUS "    Build Doc  : ${ANXI_BUILD_DOC}")
message(STATUS "    Build Test  : ${ANXI_BUILD_TEST}")
message(STATUS "=======================================================")

# anxi app
add_subdirectory(app)

# examples
if(ANXI_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# test
if(ANXI_BUILD_TEST)
    add_subdirectory(test)
endif()

# doc
if(ANXI_BUILD_DOC)
    add_subdirectory(doc)
endif()

# third_party
add_subdirectory(third_party/CSerialPort)

if(WIN32)
    add_subdirectory(third_party/duilib)
endif()

add_subdirectory(third_party/dmgraph)
add_subdirectory(third_party/sqlite)
add_subdirectory(third_party/tinyxml2)

# ##############################################################################
# install target
include(install)
include(cpack_nsis_setup)