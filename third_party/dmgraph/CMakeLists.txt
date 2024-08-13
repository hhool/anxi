# ********************************************************************
# CMakeLists for anxi
# file: CMakeLists.txt
# author: hhool
# date: 2024-08-16
# ********************************************************************

cmake_minimum_required(VERSION 3.0)

option(USE_UNICODE "Use unicode" ON)
option(USE_SHARED "Use shared library" ON)

add_subdirectory(source)
