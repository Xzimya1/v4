#!/bin/bash
set -e
echo "SleepGuard / Somni - macOS build"

if ! command -v cmake >/dev/null 2>&1; then
  echo "CMake not found. Install CMake and Qt 6 first."
  exit 1
fi

mkdir -p build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release

echo
echo "Build complete."
echo "Executable: build/SleepGuard"
