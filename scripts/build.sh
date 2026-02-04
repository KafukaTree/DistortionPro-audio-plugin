#!/bin/bash
# DistortionPro Build Script for macOS
# Requires: Xcode 12+ or CMake 3.16+, JUCE

set -e

echo "============================================"
echo "DistortionPro - macOS Build Script"
echo "============================================"

# Configuration
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_ROOT="$( cd "$SCRIPT_DIR/.." && pwd )"
BUILD_DIR="$PROJECT_ROOT/build"
CONFIG="Release"

# Check for JUCE
if [ -z "$JUCE_PATH" ]; then
    echo "ERROR: JUCE_PATH environment variable not set"
    echo "Please set JUCE_PATH to your JUCE installation directory"
    echo "Example: export JUCE_PATH=/Users/user/JUCE"
    exit 1
fi

echo "JUCE Path: $JUCE_PATH"
echo "Project Root: $PROJECT_ROOT"
echo "Build Directory: $BUILD_DIR"

# Create build directory
mkdir -p "$BUILD_DIR"

echo ""
echo "Configuring CMake..."
cd "$BUILD_DIR"

cmake .. -G "Xcode" \
    -DCMAKE_BUILD_TYPE=$CONFIG \
    -DJUCE_PATH="$JUCE_PATH" \
    -DCMAKE_INSTALL_PREFIX="$PROJECT_ROOT/install"

echo ""
echo "Building DistortionPro..."
cmake --build . --config $CONFIG -j$(sysctl -n hw.ncpu)

if [ $? -ne 0 ]; then
    echo "Build failed!"
    exit 1
fi

echo ""
echo "Build completed successfully!"

# Copy outputs
OUTPUT_DIR="$BUILD_DIR/$CONFIG"
INSTALL_DIR="$PROJECT_ROOT/install/VST3"
AAX_DIR="$PROJECT_ROOT/install/AAX"

mkdir -p "$INSTALL_DIR"
mkdir -p "$AAX_DIR"

echo ""
echo "Copying plugin files..."

# Copy VST3
if [ -d "$OUTPUT_DIR/DistortionPro.vst3" ]; then
    echo "Copying VST3 plugin..."
    cp -R "$OUTPUT_DIR/DistortionPro.vst3" "$INSTALL_DIR/"
fi

# Copy AAX if available
if [ -d "$OUTPUT_DIR/DistortionPro.aax" ]; then
    echo "Copying AAX plugin..."
    cp -R "$OUTPUT_DIR/DistortionPro.aax" "$AAX_DIR/"
fi

echo ""
echo "============================================"
echo "Build Complete!"
echo "============================================"
echo "Output directory: $OUTPUT_DIR"
echo "VST3 install: $INSTALL_DIR"
echo "AAX install: $AAX_DIR"
echo ""
echo "To install VST3, run:"
echo "  cp -R $INSTALL_DIR/DistortionPro.vst3 ~/Library/Audio/Plug-Ins/VST3/"
echo ""
echo "To sign VST3:"
echo "  codesign --force --sign - --entitlements=Entitlements.plist DistortionPro.vst3"
echo ""
echo "To notarize (requires Apple Developer account):"
echo "  xcrun altool --notarize-app --primary-bundle-id com.yourname.distortionpro \\"
echo "    --username YOUR_APPLE_ID --password YOUR_APP_SPECIFIC_PASSWORD \\"
echo "    --file DistortionPro.zip"
