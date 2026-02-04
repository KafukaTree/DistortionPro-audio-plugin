@echo off
REM Install open-source VST2 headers

echo Installing VST2 headers...

REM Create directory
mkdir "%~dp0..\vst2sdk"

REM Download VST2 headers from open source
echo Downloading VST2 headers...
git clone https://github.com/soundsearch/vestige.git "%~dp0..\vst2sdk" --depth 1 --single-branch

if %ERRORLEVEL% neq 0 (
    echo Error downloading VST2 headers
    pause
    exit /b 1
)

echo.
echo VST2 headers installed!
echo Location: %~dp0..\vst2sdk
pause
