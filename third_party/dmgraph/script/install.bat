::windows script to install the dmgraph command line tool
::this script will register the dmgraph com dll and unregister it
::run this script as administrator.

::register the dmgraph com dll
:: start command prompt as administrator with the following command
:: refer to the following link for more information
:: https://superuser.com/questions/788924/is-it-possible-to-automatically-run-a-batch-file-as-administrator

@echo off

:: BatchGotAdmin
:-------------------------------------
REM  --> Check for permissions
>nul 2>&1 "%SYSTEMROOT%\system32\cacls.exe" "%SYSTEMROOT%\system32\config\system"

REM --> If error flag set, we do not have admin.
if '%errorlevel%' NEQ '0' (
    echo Requesting administrative privileges...
    goto UACPrompt
) else ( goto gotAdmin )

:UACPrompt
    echo Set UAC = CreateObject^("Shell.Application"^) > "%temp%\getadmin.vbs"
    set params = %*:"=""
    echo UAC.ShellExecute "cmd.exe", "/c %~s0 %params%", "", "runas", 1 >> "%temp%\getadmin.vbs"

    "%temp%\getadmin.vbs"
    del "%temp%\getadmin.vbs"
    exit /B

:gotAdmin
    pushd "%CD%"
    CD /D "%~dp0"
:--------------------------------------

:: how to run the script
:: path to the script with the script args --register or --unregister
:: path to install.bat --register path to dmgraph.dll
:: path to install.bat --unregister path to dmgraph.dll
:: if the path to the dmgraph.dll file is not provided, the script will regsiter the dmgraph.dll in the current directory

:: get the arguments
if "%1"=="" (
    :: if no arguments are provided, register the dmgraph.dll in the current directory
    set arg1=--register
    set arg2=dmgraph.dll
) else (
    :: if arguments are provided, get the arguments
    set arg1=%1
    set arg2=%2
)

:: check if the argument is --register
if "%arg1%"=="--register" (
    if "%arg2%"=="" (
        :: if the path to the dmgraph.dll file is not provided, register the dmgraph.dll in the current directory
        set arg2=dmgraph.dll
    )
    if not exist %arg2% (
        :: if the dmgraph.dll file does not exist, exit the script
        :: echo the dmgraph.dll file does not exist with warning color
        echo dmgraph.dll file does not exist
        goto end_script
    )
    :: register the dmgraph.dll file with regsvr32 and check if it is registered
    regsvr32 %arg2%
    echo dmgraph.dll registered
) else (
    :: if the argument is not --register, check if the argument is --unregister
    if "%arg1%"=="--unregister" (
        if "%arg2%"=="" (
             :: if the path to the dmgraph.dll file is not provided, register the dmgraph.dll in the current directory
             set arg2=dmgraph.dll
        )
        if not exist %arg2% (
            :: if the dmgraph.dll file does not exist, exit the script
            echo dmgraph.dll file does not exist
            goto end_script
        )
        :: unregister the dmgraph.dll
        regsvr32 /u %arg2%
        echo dmgraph.dll unregistered
    )
)

:end_script
:: pause the script to keep the command prompt open to view the output
:: keep the command prompt open to view the output
echo Press any key to exit...
pause > nul

:: end of script