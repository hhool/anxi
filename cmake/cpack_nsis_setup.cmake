# script with --config package that packaging for publish
# cmake --build out\Release --config Release --target package
include(CMakePackageConfigHelpers)
include(InstallRequiredSystemLibraries)
set(CPACK_GENERATOR NSIS)
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VENDOR "anxi")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "anxi")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "anxi")
set(CPACK_PACKAGE_EXECUTABLES "anxi" "anxi")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}-win32")
set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/package/LICENSE")

set(CPACK_NSIS_DISPLAY_NAME "anxi")
set(CPACK_NSIS_PACKAGE_NAME "anxi")
set(CPACK_NSIS_UNINSTALL_NAME "Uninstall")
#set(CPACK_NSIS_HELP_LINK "https://www.anxi.com")
#set(CPACK_NSIS_URL_INFO_ABOUT "https://www.anxi.com")
set(CPACK_NSIS_CONTACT "hhool@outlook.com")
set(CPACK_NSIS_MODIFY_PATH ON)
set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL OFF)
#set(CPACK_NSIS_MENU_LINKS "https://www.anxi.com" "anxi")
set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES")
# install command
# shortcut from desktop and start menu to bin directory
set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "SetOutPath '$INSTDIR\\\\bin'")
list(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "CreateShortCut '$DESKTOP\\\\anxi.lnk' '$INSTDIR\\\\bin\\\\anxi.exe'")
list(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "CreateShortCut '$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\anxi.lnk' '$INSTDIR\\\\bin\\\\anxi.exe'")
# register  regsvr32 /s dmgraph.dll
list(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "RegDLL '$INSTDIR\\\\bin\\\\dmgraph.dll'")
string (REPLACE ";" "\n" CPACK_NSIS_EXTRA_INSTALL_COMMANDS "${CPACK_NSIS_EXTRA_INSTALL_COMMANDS}")
# uninstall command
# delete shortcut from desktop and start menu
set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "Delete '$DESKTOP\\\\anxi.lnk'")
list(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "Delete '$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\anxi.lnk'")
# unregister  regsvr32 /u /s dmgraph.dll
list(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "UnRegDLL '$INSTDIR\\\\bin\\\\dmgraph.dll'")
string (REPLACE ";" "\n" CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "${CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS}")

include(CPackComponent)
cpack_add_component(binaries DISPLAY_NAME "anxi" DESCRIPTION "anxi binaries" REQUIRED)
set(CPACK_COMPONENTS_ALL binaries)
set(CPACK_COMPONENT_UNSPECIFIED_HIDDEN TRUE)
set(CPACK_COMPONENT_UNSPECIFIED_REQUIRED OFF)
set(CPACK_COMPONENTS_ALL_SET_BY_USER OFF)
set(CPACK_SET_DESTDIR OFF)

include(CPack)