#!/bin/bash
set -euo pipefail

APP_NAME="SleepGuard.app"
BUILD_DIR="build"
APP_DIR="$BUILD_DIR/$APP_NAME"
DMG_PATH="$BUILD_DIR/SleepGuard.dmg"
RAW_DMG_PATH="$BUILD_DIR/$APP_NAME.dmg"

if [ ! -f "$BUILD_DIR/SleepGuard" ]; then
  echo "Build the project first."
  exit 1
fi

if ! command -v macdeployqt >/dev/null 2>&1; then
  echo "macdeployqt not found. Install Qt 6 or run inside a Qt-enabled shell."
  exit 1
fi

rm -rf "$APP_DIR" "$DMG_PATH" "$RAW_DMG_PATH"
mkdir -p "$APP_DIR/Contents/MacOS"
cp "$BUILD_DIR/SleepGuard" "$APP_DIR/Contents/MacOS/SleepGuard"

cat > "$APP_DIR/Contents/Info.plist" <<'PLIST'
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN"
 "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>CFBundleName</key><string>SleepGuard</string>
    <key>CFBundleDisplayName</key><string>SleepGuard</string>
    <key>CFBundleIdentifier</key><string>com.sleepguard.app</string>
    <key>CFBundleExecutable</key><string>SleepGuard</string>
    <key>CFBundlePackageType</key><string>APPL</string>
    <key>CFBundleShortVersionString</key><string>0.1</string>
    <key>CFBundleVersion</key><string>0.1</string>
</dict>
</plist>
PLIST

(cd "$BUILD_DIR" && macdeployqt "$APP_NAME" -always-overwrite -dmg)

if [ -f "$RAW_DMG_PATH" ]; then
  mv "$RAW_DMG_PATH" "$DMG_PATH"
fi

if [ ! -f "$DMG_PATH" ]; then
  echo "macdeployqt did not create $DMG_PATH"
  find "$BUILD_DIR" -maxdepth 2 -type f -name '*.dmg' -print
  exit 1
fi

echo
echo "macOS app: $APP_DIR"
echo "DMG: $DMG_PATH"
