# ********************************************************************
# CMakeLists for anxi
# file: CMakeLists.txt
# author: hhool
# date: 2024-07-27
# ********************************************************************

cmake_minimum_required(VERSION 3.0)

project(test LANGUAGES CXX)

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

set(PROJECT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/..")
message(STATUS "PROJECT_PATH: ${PROJECT_PATH}")
include_directories(${PROJECT_PATH})

# gtest library path
include(ExternalProject)

# Build gtest with UNICODE define for windows platform
if(WIN32)
    add_definitions(-DUNICODE -D_UNICODE)
endif()

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# ###############
# Define a test
add_executable(json_unittest json_unittest.cc)
target_include_directories(json_unittest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/jsoncpp/source/include)

# Link gtest
target_link_libraries(json_unittest gtest_main gtest jsoncpp_static)
