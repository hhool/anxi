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

set(COMMON_FILES
    common/crc16.cc
    common/crc16.h
    common/defines.h
    common/file_utils.cc
    common/file_utils.h
    common/logger.cc
    common/logger.h
    common/module_utils.cc
    common/module_utils.h
    common/string_utils.cc
    common/string_utils.h
    common/thread.cc
    common/thread.h
    common/time_utils.cc
    common/time_utils.h)
source_group("common" FILES ${COMMON_FILES})
list(APPEND APP_SOURCES ${COMMON_FILES})

set(DB_FILES
    db/database_factory.cc
    db/database_factory.h
    db/database_helper.cc
    db/database_helper.h
    db/database_impl.cc
    db/database_impl.h
    db/database.cc
    db/database.h)
source_group("db" FILES ${DB_FILES})
list(APPEND APP_SOURCES ${DB_FILES})

set(DEVICE_FILES
    device/device_com_factory.cc
    device/device_com_factory.h
    device/device_com_impl.cc
    device/device_com_impl.h
    device/device_com_settings_helper.cc
    device/device_com_settings_helper.h
    device/device_com_settings.cc
    device/device_com_settings.h
    device/device_com.cc
    device/device_com.h
    device/device_exp_amplitude_settings.cc
    device/device_exp_amplitude_settings.h
    device/device_exp_data_sample_settings.cc
    device/device_exp_data_sample_settings.h
    device/device_exp_graph_settings.cc
    device/device_exp_graph_settings.h
    device/device_exp_load_static_settings.cc
    device/device_exp_load_static_settings.h
    device/device_exp_ultrasound_settings.cc
    device/device_exp_ultrasound_settings.h)

source_group("device" FILES ${DEVICE_FILES})
list(APPEND APP_SOURCES ${DEVICE_FILES})

if(WIN32)
    set(DEVICE_STLOAD_FILES
        device/stload/stload_common.h
        device/stload/stload_helper.cc
        device/stload/stload_helper.h
        device/stload/stload_wrapper.cc
        device/stload/stload_wrapper.h)
    source_group("device/stload" FILES ${DEVICE_STLOAD_FILES})
    list(APPEND APP_SOURCES ${DEVICE_STLOAD_FILES})

    if(ANXI_BUILD_STLOAD_SIMULATOR)
        set(DEVICE_STLOAD_SIMULATOR_FILES
            device/stload_simulation/stload_device.cc
            device/stload_simulation/stload_device.def
            device/stload_simulation/stload_device.h)
        source_group("device/stload_simulator" FILES ${DEVICE_STLOAD_SIMULATOR_FILES})
        add_library(app_stload_simulator SHARED ${DEVICE_STLOAD_SIMULATOR_FILES})
        target_link_libraries(app_stload_simulator app_ui)
        target_compile_definitions(app_stload_simulator PRIVATE STLOAD_SIMULATION_EXPORTS)
    endif()

    set(DEVICE_ULTRASONIC_FILES
        device/ultrasonic/ultra_device.cc
        device/ultrasonic/ultra_device.h
        device/ultrasonic/ultra_helper.cc
        device/ultrasonic/ultra_helper.h)
    source_group("device/ultrasonic" FILES ${DEVICE_ULTRASONIC_FILES})
    list(APPEND APP_SOURCES ${DEVICE_ULTRASONIC_FILES})

    if(ANXI_BUILD_UNITTEST)
        set(APP_DEVICE_ULTRASONIC_UNITTEST_FILES
            device/ultrasonic/ultra_device_unittest.cc)
        source_group("device_utral_unittest" FILES ${APP_DEVICE_ULTRASONIC_UNITTEST_FILES})
        list(APPEND APP_DEVICE_ULTRASONIC_UNITTEST_FILES ${APP_DEVICE_ULTRASONIC_UNITTEST_FILES})
        add_executable(app_device_ultra_unittest ${APP_DEVICE_ULTRASONIC_UNITTEST_FILES})
        target_link_libraries(app_device_ultra_unittest gtest_main gtest app_ui)
    endif()
endif()

if(ANXI_BUILD_UNITTEST)
    if(WIN32)
        set(APP_DEVICE_UNITTEST_FILES
            device/stload/stload_wrapper_unittest.cc)
        source_group("device_stload_unittest" FILES ${APP_DEVICE_UNITTEST_FILES})
        list(APPEND APP_DEVICE_UNITTEST_FILES ${APP_DEVICE_UNITTEST_FILES})
        add_executable(app_device_stload_unittest ${APP_DEVICE_UNITTEST_FILES})
        target_link_libraries(app_device_stload_unittest gtest_main gtest app_ui)
    endif()
endif()

set(EXPDATA_FILES
    expdata/experiment_data_base.cc
    expdata/experiment_data_base.h
    expdata/LibOb_strptime.c
    expdata/LibOb_strptime.h)

if(WIN32)
    list(APPEND EXPDATA_FILES
        expdata/strptime.c
        expdata/strptime.h)
endif()

source_group("expdata" FILES ${EXPDATA_FILES})
list(APPEND APP_SOURCES ${EXPDATA_FILES})

set(ESOLUTION_FILES
    esolution/solution_design_default.cc
    esolution/solution_design_default.h
    esolution/solution_design_helper.cc
    esolution/solution_design_helper.h
    esolution/solution_design.cc
    esolution/solution_design.h)
source_group("esolution" FILES ${ESOLUTION_FILES})
list(APPEND APP_SOURCES ${ESOLUTION_FILES})

if(ANXI_BUILD_UNITTEST)
    set(APP_ESOLUTION_UNITTEST_FILES
        esolution/solution_design_default_unittest.cc)
    source_group("esoution_unittest" FILES ${APP_ESOLUTION_UNITTEST_FILES})
    list(APPEND APP_ESOLUTION_UNITTEST_SOURCES ${APP_ESOLUTION_UNITTEST_FILES})
    add_executable(app_esolution_unittest ${APP_ESOLUTION_UNITTEST_FILES})
    target_link_libraries(app_esolution_unittest gtest_main gtest app_ui)
endif()

set(ESOLUTION_ALG_FILES
    esolution/algorithm/alg_th3.cc
    esolution/algorithm/alg.h)
source_group("esolution\\algorithm" FILES ${ESOLUTION_ALG_FILES})
list(APPEND APP_SOURCES ${ESOLUTION_ALG_FILES})

if(ANXI_BUILD_UNITTEST)
    set(APP_ESOLUTION_ALG_UNITTEST_FILES
        esolution/algorithm/alg_unittest.cc)
    source_group("algorithm_unittest" FILES ${APP_ESOLUTION_ALG_UNITTEST_FILES})
    list(APPEND APP_ESOLUTION_ALG_UNITTEST_SOURCES ${APP_ESOLUTION_ALG_UNITTEST_FILES})
    add_executable(app_esolution_alg_unittest ${APP_ESOLUTION_ALG_UNITTEST_FILES})
    target_link_libraries(app_esolution_alg_unittest gtest_main gtest app_ui)
endif()

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
        ui/dialog_amplitude_calibration_settings.cc
        ui/dialog_amplitude_calibration_settings.h
        ui/dialog_com_port_settings.cc
        ui/dialog_com_port_settings.h
        ui/dialog_com_record_2000c.cc
        ui/dialog_com_record_2000c.h
        ui/dialog_common.cc
        ui/dialog_common.h
        ui/dialog_exp_data_record.cc
        ui/dialog_exp_data_record.h
        ui/dialog_static_load_guaranteed_settings.cc
        ui/dialog_static_load_guaranteed_settings.h
        ui/main_window.cc
        ui/main_window.h
        ui/ui_constants.cc
        ui/ui_constants.h
        ui/ui_virtual_wnd_base.h
        ui/work_window_menu_design.cc
        ui/work_window_menu_design.h
        ui/work_window_menu_shadow.cc
        ui/work_window_menu_shadow.h
        ui/work_window_menu_store.cc
        ui/work_window_menu_store.h
        ui/work_window_status_bar.cc
        ui/work_window_status_bar.h
        ui/work_window_tab_main_page_base.h
        ui/work_window_tab_main_first_page_solution_design_base.h
        ui/work_window_tab_main_first_page_solution_design.cc
        ui/work_window_tab_main_first_page_solution_design.h
        ui/work_window_tab_main_second_page_data.cc
        ui/work_window_tab_main_second_page_data.h
        ui/work_window_tab_main_second_page_graph_ctrl.cc
        ui/work_window_tab_main_second_page_graph_ctrl.h
        ui/work_window_tab_main_second_page_graph.cc
        ui/work_window_tab_main_second_page_graph.h
        ui/work_window_tab_main_second_page_base.h
        ui/work_window_tab_main_second_page.cc
        ui/work_window_tab_main_second_page.h
        ui/work_window_tab_main_third_page.cc
        ui/work_window_tab_main_third_page.h
        ui/work_window.cc
        ui/work_window.h)
    source_group("ui" FILES ${UI_FILES})
    list(APPEND APP_SOURCES ${UI_FILES})
endif()

# resource files
if(WIN32)
    enable_language("RC")
    set_source_files_properties(${CMAKE_SOURCE_DIR}/resource.rc LANGUAGE RC)
    set(RES_FILES
        resource.rc)
    source_group("resource" FILES ${RES_FILES})

    # list(APPEND APP_SOURCES resource.rc)
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

# add library libcserialport library dependencie
target_include_directories(app_ui PRIVATE ${PROJECT_PATH}/third_party/CSerialPort/source/include)
target_link_libraries(app_ui libcserialport)
add_dependencies(app_ui libcserialport)

# add library sqlite3 library dependencie
target_include_directories(app_ui PRIVATE ${PROJECT_PATH}/third_party/sqlite/source)
target_link_libraries(app_ui SQLite::SQLite3)
add_dependencies(app_ui SQLite::SQLite3)

# ##############################################################################
# add executable
add_executable(app_exe main.cc ${RES_FILES})
add_dependencies(app_exe app_ui)
target_link_libraries(app_exe app_ui)

if(WIN32)
    target_link_libraries(app_exe user32 gdi32)
    set_target_properties(app_exe PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")

    # set working directory to bin directory
    # for visual studio debugger to find third stload third dll
    # and the resource files
    set_target_properties(app_exe PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/../bin/${CMAKE_BUILD_TYPE})
endif()

# rename the output file to product name
set_target_properties(app_exe PROPERTIES OUTPUT_NAME ${PRODUCT_NAME})

# unittest files
if(ANXI_BUILD_UNITTEST)
    set(APP_UNITTEST_FILES
        common/crc16_unittest.cc
        common/logger_unittest.cc
        db/database_unittest.cc)
    source_group("common_unittest" FILES ${APP_UNITTEST_FILES})
    list(APPEND APP_UNITTEST_SOURCES ${APP_UNITTEST_FILES})
    add_executable(app_unittest ${APP_UNITTEST_FILES})
    target_link_libraries(app_unittest app_ui gtest_main gtest)
endif()

# ##############################################################################
# copy folder resource/skin resource/default to the target directory
add_custom_command(TARGET app_exe POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/resource/pilot/etm ${PROJECT_BINARY_DIR}/../bin/${CMAKE_BUILD_TYPE}/)
add_custom_command(TARGET app_exe POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/resource/skin ${PROJECT_BINARY_DIR}/../bin/${CMAKE_BUILD_TYPE}/skin)
add_custom_command(TARGET app_exe POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/resource/default ${PROJECT_BINARY_DIR}/../bin/${CMAKE_BUILD_TYPE}/default)