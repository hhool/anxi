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

# add micro define
if(WIN32)
    add_definitions(-DUNICODE -D_UNICODE)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

set(COMMON_FILES
    common/defines.h)
source_group("common" FILES ${COMMON_FILES})
list(APPEND APP_SOURCES ${COMMON_FILES})

set(DOCUMENT_FILES
    document/document_interface.h)
source_group("document" FILES ${DOCUMENT_FILES})
list(APPEND APP_SOURCES ${DOCUMENT_FILES})

set(ESOLUTION_FILES
    esolution/solution_design.cc
    esolution/solution_design.h)
source_group("esolution" FILES ${ESOLUTION_FILES})
list(APPEND APP_SOURCES ${ESOLUTION_FILES})

set(APP_FILES
    application.cc
    application.h
    appinterface.h)
source_group("application" FILES ${APP_FILES})
list(APPEND APP_SOURCES ${APP_FILES})

# add platform specific sources
if(WIN32)
    set(UI_FILES
        ui/dialog_about.cc
        ui/dialog_about.h
        ui/dialog_com_port_settings.cc
        ui/dialog_com_port_settings.h
        ui/main_window.cc
        ui/main_window.h
        ui/ui_constants.cc
        ui/ui_constants.h
        ui/work_window_menu_design.cc
        ui/work_window_menu_design.h
        ui/work_window_menu_shadow.cc
        ui/work_window_menu_shadow.h
        ui/work_window_menu_store.cc
        ui/work_window_menu_store.h
        ui/work_window.cc
        ui/work_window.h)
    source_group("ui" FILES ${UI_FILES})
    list(APPEND APP_SOURCES ${UI_FILES})
endif()

# resource files
if(WIN32)
    set(RES_FILES
        resource.rc)
    source_group("resource" FILES ${RES_FILES})
    list(APPEND APP_SOURCES resource.rc)
endif()

# add library
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
add_library(app_ui STATIC ${APP_SOURCES})

# # add library duilib source path
target_include_directories(app_ui PUBLIC ${PROJECT_PATH})
target_include_directories(app_ui PRIVATE ${PROJECT_PATH}/third_party/duilib/source/DuiLib)
add_dependencies(app_ui duilib)

# # add library duilib library dependencie
target_link_libraries(app_ui duilib)

# add library tinyxml2 source path
target_include_directories(app_ui PRIVATE ${PROJECT_PATH}/third_party/tinyxml2/source)
target_link_libraries(app_ui tinyxml2::tinyxml2)
add_dependencies(app_ui tinyxml2::tinyxml2)

# add library duilib library dependencie
target_link_libraries(app_ui tinyxml2::tinyxml2)

# ##############################################################################
# add executable
add_executable(app_exe main.cc)
add_dependencies(app_exe app_ui)
target_link_libraries(app_exe app_ui)

if(WIN32)
    target_link_libraries(app_exe user32 gdi32)
    set_target_properties(app_exe PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
endif()

# rename the output file to product name
set_target_properties(app_exe PROPERTIES OUTPUT_NAME ${PRODUCT_NAME})

# ##############################################################################
# copy folder resource/skin resource/default to the target directory
add_custom_command(TARGET app_exe POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/resource/skin ${PROJECT_BINARY_DIR}/../bin/${CMAKE_BUILD_TYPE}/skin)
add_custom_command(TARGET app_exe POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/resource/default ${PROJECT_BINARY_DIR}/../bin/${CMAKE_BUILD_TYPE}/default)