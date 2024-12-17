set(PROGRAM_PERMISSIONS_DEFAULT
    OWNER_WRITE OWNER_READ OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE)

# install the target to the target directory
install(TARGETS app_exe DESTINATION bin COMPONENT binaries
    PERMISSIONS ${PROGRAM_PERMISSIONS_DEFAULT} SETUID)

# install files at resource/pilot/etm/ to the target directory
install(FILES ${PROJECT_SOURCE_DIR}/app/resource/pilot/etm/CTRL.dll DESTINATION bin COMPONENT binaries)
install(FILES ${PROJECT_SOURCE_DIR}/app/resource/pilot/etm/HardPara.mdb DESTINATION bin COMPONENT binaries)
install(FILES ${PROJECT_SOURCE_DIR}/app/resource/pilot/etm/TestPilot.ini DESTINATION bin COMPONENT binaries)
install(FILES ${PROJECT_SOURCE_DIR}/app/resource/pilot/etm/WanceMachine.mdb DESTINATION bin COMPONENT binaries)
install(FILES ${PROJECT_SOURCE_DIR}/app/resource/pilot/etm2/CTRL2.dll DESTINATION bin COMPONENT binaries)
install(FILES ${PROJECT_SOURCE_DIR}/app/resource/pilot/etm2/TestLeader-CN.sys DESTINATION bin COMPONENT binaries)
install(FILES ${PROJECT_SOURCE_DIR}/app/resource/pilot/etm2/WanceMachine-CN.mdb DESTINATION bin COMPONENT binaries)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/app/resource/pilot DESTINATION bin COMPONENT binaries)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/app/resource/template DESTINATION bin COMPONENT binaries)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/app/resource/default DESTINATION bin COMPONENT binaries)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/app/resource/db DESTINATION bin COMPONENT binaries)
install(FILES ${PROJECT_SOURCE_DIR}/package/anxi.bat DESTINATION bin COMPONENT binaries)
install(FILES ${PROJECT_SOURCE_DIR}/third_party/dmgraph/script/install.bat DESTINATION bin COMPONENT binaries)
install(FILES ${PROJECT_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/dmgraph.dll DESTINATION bin COMPONENT binaries)
install(FILES ${PROJECT_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/duilib.dll DESTINATION bin COMPONENT binaries)
install(FILES ${PROJECT_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/app_stload_simulator.dll DESTINATION bin COMPONENT binaries)
install(FILES ${PROJECT_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/todocx.exe DESTINATION bin COMPONENT binaries)
install(FILES ${PROJECT_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/DocumentFormat.OpenXml.dll DESTINATION bin COMPONENT binaries)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    # install files at resource/runtime/debug/ to the target directory
    install(FILES ${PROJECT_SOURCE_DIR}/app/resource/runtime/debug/ucrtbased.dll DESTINATION bin COMPONENT binaries)
    install(FILES ${PROJECT_SOURCE_DIR}/app/resource/runtime/debug/vcruntime140d.dll DESTINATION bin COMPONENT binaries)
    install(FILES ${PROJECT_SOURCE_DIR}/app/resource/runtime/debug/msvcp140d.dll DESTINATION bin COMPONENT binaries)
    install(FILES ${PROJECT_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/tinyxml2.dll DESTINATION bin COMPONENT binaries)
    install(FILES ${PROJECT_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/libcserialport.dll DESTINATION bin COMPONENT binaries)
    install(FILES ${PROJECT_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/sqlite3-dbg.dll DESTINATION bin COMPONENT binaries)
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/app/resource/skin DESTINATION bin COMPONENT binaries)
else()
    # install files at resource/runtime/release/ to the target directory
    install(FILES ${PROJECT_SOURCE_DIR}/app/resource/runtime/release/ucrtbase.dll DESTINATION bin COMPONENT binaries)
    install(FILES ${PROJECT_SOURCE_DIR}/app/resource/runtime/release/vcruntime140.dll DESTINATION bin COMPONENT binaries)
    install(FILES ${PROJECT_SOURCE_DIR}/app/resource/runtime/release/msvcp140.dll DESTINATION bin COMPONENT binaries)
    install(FILES ${PROJECT_SOURCE_DIR}/app/resource/runtime/release/msvcp_win.dll DESTINATION bin COMPONENT binaries)
    install(FILES ${PROJECT_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/tinyxml2.dll DESTINATION bin COMPONENT binaries)
    install(FILES ${PROJECT_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/libcserialport.dll DESTINATION bin COMPONENT binaries)
    install(FILES ${PROJECT_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/sqlite3.dll DESTINATION bin COMPONENT binaries)
    install(FILES ${PROJECT_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/skin/skin_default.zip DESTINATION bin/skin COMPONENT binaries)
endif()