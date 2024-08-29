# ********************************************************************
# CMakeLists for anxi
# file: CMakeLists.txt
# author: hhool
# date: 2024-07-16
# ********************************************************************

cmake_minimum_required(VERSION 3.0)

option(UILIB_STATIC ON)
option(USE_DEMO "Use demo" OFF)

add_subdirectory(source)
