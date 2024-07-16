#********************************************************************
# CMakeLists for anxi
# file: CMakeLists.txt
# author: hhool
# date: 2024-07-16
#********************************************************************

cmake_minimum_required(VERSION 3.0)

project(anxi LANGUAGES CXX)

# set output directory
if (CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
endif()

# set build tpye
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(ANXI_BUILD_EXAMPLES "Build anxi examples" ON)
option(ANXI_BUILD_DOC "Build anxi Doc" OFF)
option(ANXI_BUILD_TEST "Build anxi Test" OFF)

# only build library if add anxi as subdirectory
if (NOT CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    set(ANXI_BUILD_EXAMPLES OFF)
    set(ANXI_BUILD_DOC OFF)
    set(ANXI_BUILD_TEST OFF)
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
if (ANXI_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif ()

# test
if (ANXI_BUILD_TEST)
    add_subdirectory(test)
endif ()

# doc
if (ANXI_BUILD_DOC)
    add_subdirectory(doc)
endif ()

# third_party
add_subdirectory(third_party)
