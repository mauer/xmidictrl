@ECHO OFF

REM We expect 2 parameters
IF "%~1"=="" GOTO :PRINT_HELP
IF "%~2"=="" GOTO :PRINT_HELP

SETLOCAL
SET MY_VC_BASE=%~1
SET MY_BUILD_DIR=%~2

REM Set VC environment variables
CALL "%MY_VC_BASE%\VC\Auxiliary\Build\vcvarsall.bat" x64
IF %ERRORLEVEL% NEQ 0 EXIT /B %ERRORLEVEL%

REM Create an empty directory to build in
IF exist "%MY_BUILD_DIR%\" RMDIR /Q /S "%MY_BUILD_DIR%"
MKDIR "%MY_BUILD_DIR%""
IF %ERRORLEVEL% NEQ 0 EXIT /B %ERRORLEVEL%
PUSHD "%MY_BUILD_DIR%"
IF %ERRORLEVEL% NEQ 0 EXIT /B %ERRORLEVEL%
ECHO I am now in: %CD%

REM Create Makefile from CMakeLists.txt, using NMake output because NMake happens to be available
REM CMAKE -G "NMake Makefiles" -DCMAKE_BUILD_TYPE:STRING="RelWithDebInfo" -DCMAKE_INSTALL_PREFIX:PATH="%CD%\install\x64-Release" -DCMAKE_CXX_FLAGS:STRING="/DWIN32 /D_WINDOWS /DWIN32_LEAN_AND_MEAN /D_CRT_SECURE_NO_WARNINGS /GS- /Gd /Gy- /Zc:wchar_t /Zc:forScope /Zc:inline /Zc:__cplusplus /W4 /EHsc /fp:precise /diagnostics:caret" -DCMAKE_CXX_FLAGS_RELWITHDEBINFO:STRING="/MD /Zi /O2 /DNDEBUG" -DCMAKE_C_COMPILER:FILEPATH="cl.exe" -DCMAKE_CXX_COMPILER:FILEPATH="cl.exe"  -DCMAKE_MAKE_PROGRAM="nmake.exe" ..
CMAKE -G "NMake Makefiles" -DCMAKE_BUILD_TYPE:STRING="RelWithDebInfo" -DCMAKE_MAKE_PROGRAM="nmake.exe" -DCMAKE_TOOLCHAIN_FILE="..\docker\Toolchain-vs-x86-64.cmake" ..
IF %ERRORLEVEL% NEQ 0 EXIT /B %ERRORLEVEL%

REM Perform the actual build
NMAKE /A
IF %ERRORLEVEL% NEQ 0 EXIT /B %ERRORLEVEL%

ECHO Windows Build: SUCCESS!
POPD
EXIT /B 0

:PRINT_HELP
ECHO Call with two parameters:
ECHO 1. Base directory for Visual Studio Build Tools, something like
ECHO    "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community" or
ECHO    "C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools";
ECHO 2. Build directory, usually something local like "build-win".
EXIT /B 1
