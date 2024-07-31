#********************************************************************
# CMakeLists for anxi
# file: CMakeLists.txt
# author: hhool
# date: 2024-07-16
#********************************************************************

cmake_minimum_required(VERSION 3.0)

option(USE_UNICODE "Use unicode" ON)
option(USE_SHARED "Use shared library" ON)
option(JSONCPP_WITH_TESTS "Build jsoncpp tests" OFF)
option(JSONCPP_WITH_POST_BUILD_UNITTEST "Run jsoncpp tests after build" OFF)
option(JSONCPP_WITH_WARNING_AS_ERROR "Treat warnings as errors" OFF)
option(JSONCPP_WITH_PKGCONFIG_SUPPORT "Build jsoncpp with pkgconfig support" OFF)
option(JSONCPP_WITH_CMAKE_PACKAGE "Build jsoncpp with cmake package" OFF)
option(JSONCPP_WITH_EXAMPLE "Build jsoncpp example" OFF)
add_subdirectory(source)
