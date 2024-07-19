#********************************************************************
# CMakeLists for anxi
# file: CMakeLists.txt
# author: hhool
# date: 2024-07-16
#********************************************************************

cmake_minimum_required(VERSION 3.0)

set(USE_UNICODE "Use unicode" ON)
set(USE_SHARED "Use shared library" ON)
set(JSONCPP_WITH_TESTS "Build jsoncpp tests" OFF)
set(JSONCPP_WITH_POST_BUILD_UNITTEST "Run jsoncpp tests after build" OFF)
set(JSONCPP_WITH_WARNING_AS_ERROR "Treat warnings as errors" ON)
set(JSONCPP_WITH_PKGCONFIG_SUPPORT "Build jsoncpp with pkgconfig support" OFF)
add_subdirectory(source)
