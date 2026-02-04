@echo off
REM DistortionPro Build Script for Windows
REM Requires: Visual Studio 2019+ or CMake 3.16+

echo ============================================
echo DistortionPro - Windows Build Script
echo ============================================

setlocal

REM Configuration
set PROJECT_ROOT=%~dp0
set BUILD_DIR=%PROJECT_ROOT%build
set CONFIG=Release

REM Check for JUCE
if not exist "%JUCE_PATH%" (
    echo ERROR: JUCE_PATH environment variable not set
    echo Please set JUCE_PATH to your JUCE installation directory
    exit /b 1
)

echo JUCE Path: %JUCE_PATH%

REM Create build directory
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

echo.
echo Configuring CMake...
cd "%BUILD_DIR%"
cmake .. -G "Visual Studio 17 2022" -A x64 ^
    -DCMAKE_BUILD_TYPE=%CONFIG% ^
    -DJUCE_PATH="%JUCE_PATH%" ^
    -DCMAKE_INSTALL_PREFIX="%PROJECT_ROOT%install"

if %ERRORLEVEL% NEQ 0 (
    echo CMake configuration failed!
    exit /b 1
)

echo.
echo Building DistortionPro...
cmake --build . --config %CONFIG% -j%NUMBER_OF_PROCESSORS%

if %ERRORLEVEL% NEQ 0 (
    echo Build failed!
    exit /b 1
)

echo.
echo Build completed successfully!

REM Copy outputs
echo.
echo Copying plugin files...

set OUTPUT_DIR=%BUILD_DIR%\%CONFIG%
set INSTALL_DIR=%PROJECT_ROOT%install\VST3

if not exist "%INSTALL_DIR%" mkdir "%INSTALL_DIR%"

echo Copying VST3 plugin...
if exist "%OUTPUT_DIR%\DistortionPro.vst3" (
    xcopy /E /I "%OUTPUT_DIR%\DistortionPro.vst3" "%INSTALL_DIR%\DistortionPro.vst3" 2>nul || echo VST3 not found, building VST3 format...
)

echo.
echo ============================================
echo Build Complete!
echo ============================================
echo Output directory: %OUTPUT_DIR%
echo Install directory: %INSTALL_DIR%
echo.
echo To install, copy the .vst3 folder to:
echo   C:\Program Files\VST3\
echo.
echo For signing, use signtool.exe:
echo   signtool sign /fd SHA256 /tr http://timestamp.digicert.com /td SHA256 ^
echo     /a DistortionPro.vst3

endlocal
pause
