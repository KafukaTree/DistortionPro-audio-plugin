@echo off
REM Install VST3 SDK for JUCE plugin build

echo ===== Installing VST3 SDK =====

REM Create SDK directory
if not exist "%~dp0..\vst3sdk" mkdir "%~dp0..\vst3sdk"

echo Downloading VST3 SDK...
REM Download SDK from GitHub (Steinberg's official SDK)
git clone https://github.com/steinbergmedia/vst3sdk.git "%~dp0..\vst3sdk" --depth 1 --branch vstsdk3.7.5 --single-branch

if %ERRORLEVEL% neq 0 (
    echo Error: Failed to download VST3 SDK
    echo Please manually download from: https://www.steinberg.net/developers/
    pause
    exit /b 1
)

echo.
echo VST3 SDK installed successfully!
echo Path: %~dp0..\vst3sdk
echo.
echo Next steps:
echo 1. Run: cmake .. -DJUCE_PATH=..\JUCE -DVST3_SDK_PATH=..\vst3sdk -DCMAKE_BUILD_TYPE=Release
echo 2. Run: cmake --build . --config Release -j4
echo.
pause
