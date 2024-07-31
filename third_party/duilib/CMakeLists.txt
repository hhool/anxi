#********************************************************************
# CMakeLists for anxi
# file: CMakeLists.txt
# author: hhool
# date: 2024-07-16
#********************************************************************

cmake_minimum_required(VERSION 3.0)

option(USE_COPY_RESOURCES "Copy resources to bin directory" OFF)
option(USE_COPY_DLL "Copy dll to bin directory" OFF)
option(USE_UNICODE "Use unicode" ON)
option(USE_SHARED "Use shared library" ON)
option(USE_DEMO "Use demo" OFF)

add_subdirectory(source)
