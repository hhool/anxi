# ********************************************************************
# CMakeLists for anxi
# file: CMakeLists.txt
# author: hhool
# date: 2024-07-16
# ********************************************************************

cmake_minimum_required(VERSION 3.0)

project(app LANGUAGES CXX)

# set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# source files
set(PROJECT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/..")
message(STATUS "PROJECT_PATH: ${PROJECT_PATH}")
include_directories(${PROJECT_PATH})

list(APPEND APP_SOURCES
    appinterface.h
    application.cc
    application.h)

# add platform specific sources
if(WIN32)
    list(APPEND APP_SOURCES
        ui/dialog_com_port_settings.cc
        ui/dialog_com_port_settings.h
        ui/main_window.cc
        ui/main_window.h
        ui/work_window.cc
        ui/work_window.h)
endif()

# resource files
if(WIN32)
    list(APPEND APP_SOURCES resource.rc)
endif()

# add library
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
add_library(app_ui STATIC ${APP_SOURCES})

## add library duilib source path
target_include_directories(app_ui PUBLIC ${PROJECT_PATH})
target_include_directories(app_ui PRIVATE ${PROJECT_PATH}/third_party/duilib/source/DuiLib)
add_dependencies(app_ui duilib)

## add library duilib library dependencie
target_link_libraries(app_ui duilib)

###############################################################################
# add executable
add_executable(app_exe main.cc)
add_dependencies(app_exe app_ui)
target_link_libraries(app_exe app_ui)

if (WIN32)
    target_link_libraries(app_exe user32 gdi32)
    set_target_properties(app_exe PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
endif()

#rename the output file to product name
set_target_properties(app_exe PROPERTIES OUTPUT_NAME ${PRODUCT_NAME})

