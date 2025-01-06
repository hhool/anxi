# ********************************************************************
# CMakeLists for anxi
# file: CMakeLists.txt
# author: hhool
# date: 2024-07-27
# ********************************************************************

cmake_minimum_required(VERSION 3.0)

project(test LANGUAGES CXX)

set(PROJECT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/..")
message(STATUS "PROJECT_PATH: ${PROJECT_PATH}")
include_directories(${PROJECT_PATH})

set(PREFIX "anxi")

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
add_executable(${PREFIX}_tinyxml2_unittest tinyxml2_unittest.cc)
target_include_directories(${PREFIX}_tinyxml2_unittest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/tinyxml2/source)

# Link gtest
target_link_libraries(${PREFIX}_tinyxml2_unittest gtest_main gtest tinyxml2)
set_target_properties(${PREFIX}_tinyxml2_unittest PROPERTIES FOLDER "test")

# ###############
# Define a test
add_executable(${PREFIX}_cserialport_unittest cserialport_unittest.cc)
target_include_directories(${PREFIX}_cserialport_unittest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/CSerialPort/source/include)

# Link gtest
target_link_libraries(${PREFIX}_cserialport_unittest gtest_main gtest libcserialport)
set_target_properties(${PREFIX}_cserialport_unittest PROPERTIES FOLDER "test")

# ###############
# Define a test
if(WIN32)
    add_executable(${PREFIX}_ole_vartime_unittest ole_vartime_unittest.cc)
    target_link_libraries(${PREFIX}_ole_vartime_unittest gtest_main gtest comctl32 oleaut32)
    add_executable(${PREFIX}_library_win_unittest library_win_unittest.cc)
    target_link_libraries(${PREFIX}_library_win_unittest gtest_main gtest app_ui comctl32 oleaut32)
    set_target_properties(${PREFIX}_ole_vartime_unittest PROPERTIES FOLDER "test")
    set_target_properties(${PREFIX}_library_win_unittest PROPERTIES FOLDER "test")
endif()

# ###############
# Define a test
add_executable(${PREFIX}_sqlite_unittest sqlite_unittest.cc)
target_include_directories(${PREFIX}_sqlite_unittest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/sqlite/source/source)

# Link gtest
target_link_libraries(${PREFIX}_sqlite_unittest gtest_main gtest SQLite::SQLite3)
set_target_properties(${PREFIX}_sqlite_unittest PROPERTIES FOLDER "test")

# ###############
# Define a test
add_executable(${PREFIX}_usb_auth_unittest usb_auth_unittest.cc)
target_include_directories(${PREFIX}_usb_auth_unittest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/usb_auth/source)

# Link gtest
target_link_libraries(${PREFIX}_usb_auth_unittest gtest_main gtest)
set_target_properties(${PREFIX}_usb_auth_unittest PROPERTIES FOLDER "test")
