#!/bin/bash
# DistortionPro Installer for macOS
# Creates DMG package with notarization support

set -e

echo "============================================"
echo "DistortionPro - macOS Installer"
echo "============================================"

# Configuration
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_ROOT="$( cd "$SCRIPT_DIR/.." && pwd )"
BUILD_DIR="$PROJECT_ROOT/build"
INSTALL_DIR="$PROJECT_ROOT/install"
DMG_DIR="$PROJECT_ROOT/package"
VERSION="1.0.0"
BUNDLE_ID="com.yourname.distortionpro"

# Cleanup
echo "Cleaning up..."
rm -rf "$DMG_DIR"
rm -f "$PROJECT_ROOT/DistortionPro-${VERSION}.dmg"
mkdir -p "$DMG_DIR"

echo ""
echo "Building project first..."
cd "$BUILD_DIR"
cmake --build . --config Release -j$(sysctl -n hw.ncpu)

# Copy plugins
echo ""
echo "Copying plugins..."
mkdir -p "$DMG_DIR/VST3"
mkdir -p "$DMG_DIR/AAX"

if [ -d "$BUILD_DIR/Release/DistortionPro.vst3" ]; then
    cp -R "$BUILD_DIR/Release/DistortionPro.vst3" "$DMG_DIR/VST3/"
fi

if [ -d "$BUILD_DIR/Release/DistortionPro.aax" ]; then
    cp -R "$BUILD_DIR/Release/DistortionPro.aax" "$DMG_DIR/AAX/"
fi

# Copy presets
mkdir -p "$DMG_DIR/Presets"
cp "$PROJECT_ROOT/presets"/*.json "$DMG_DIR/Presets/" 2>/dev/null || true

# Create README
cat > "$DMG_DIR/README.txt" << 'EOF'
DistortionPro v1.0.0
====================

Installation:

VST3:
  Copy DistortionPro.vst3 to:
  ~/Library/Audio/Plug-Ins/VST3/

AAX (Pro Tools):
  Copy DistortionPro.aax to:
  ~/Library/Application Support/Avid/Audio/Plug-Ins/

Presets:
  Copy preset files to:
  ~/Library/Application Support/DistortionPro/Presets/

For more information, visit:
https://github.com/yourname/distortion-pro
EOF

# Create background image for DMG
cat > "$DMG_DIR/.background.png" << 'EOF'
# Create a simple gradient background using sips (macOS built-in)
# This is a placeholder - replace with actual background image
EOF

echo ""
echo "Creating DMG package..."

# Create temporary DMG
DMG_NAME="DistortionPro-${VERSION}"
DMG_TEMP="/tmp/${DMG_NAME}-temp.dmg"
DMG_FINAL="${PROJECT_ROOT}/${DMG_NAME}.dmg"

# Create the DMG
hdiutil create -volname "$DMG_NAME" \
    -srcfolder "$DMG_DIR" \
    -ov -format UDZO \
    "$DMG_TEMP"

mv "$DMG_TEMP" "$DMG_FINAL"

echo ""
echo "============================================"
echo "DMG Package Created!"
echo "============================================"
echo "File: $DMG_FINAL"
echo ""
echo "Next steps:"
echo "1. Test the DMG by mounting it"
echo "2. Code sign: codesign --force --sign - DistortionPro-${VERSION}.dmg"
echo "3. Notarize: xcrun altool --notarize-app ..."
echo "4. Staple: xcrun stapler staple DistortionPro-${VERSION}.dmg"
echo ""
echo "To mount the DMG:"
echo "  open '$DMG_FINAL'"
