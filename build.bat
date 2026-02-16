@echo off
REM Build script for AlongSight on Windows

echo Building AlongSight...
echo.

REM Create build directory if it doesn't exist
if not exist build mkdir build

REM Navigate to build directory
cd build

REM Configure with CMake
echo Configuring project with CMake...
cmake ..

if %errorlevel% neq 0 (
    echo CMake configuration failed!
    exit /b %errorlevel%
)

echo.
echo Building project...
cmake --build . --config Release

if %errorlevel% neq 0 (
    echo Build failed!
    exit /b %errorlevel%
)

echo.
echo Build completed successfully!
echo Executable location: build\Release\AlongSight.exe
echo.
pause
