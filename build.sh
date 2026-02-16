#!/bin/bash

# Build script for AlongSight on Linux/macOS

echo "Building AlongSight..."
echo ""

# Create build directory if it doesn't exist
mkdir -p build

# Navigate to build directory
cd build

# Configure with CMake
echo "Configuring project with CMake..."
cmake ..

if [ $? -ne 0 ]; then
    echo "CMake configuration failed!"
    exit 1
fi

echo ""
echo "Building project..."
cmake --build . --config Release -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

if [ $? -ne 0 ]; then
    echo "Build failed!"
    exit 1
fi

echo ""
echo "Build completed successfully!"
echo "Executable location: build/AlongSight_artefacts/AlongSight"
echo ""
