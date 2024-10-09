#********************************************************************
# CMakeLists for anxi
# file: CMakeLists.txt
# author: hhool
# date: 2024-07-29
#********************************************************************

cmake_minimum_required(VERSION 3.0)

if (CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    set(BUILD_TESTING OFF)
endif()

add_subdirectory(source)
