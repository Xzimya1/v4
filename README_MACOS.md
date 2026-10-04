# SleepGuard — macOS

## Требования
- macOS
- Qt 6 с Core, Widgets, Bluetooth, Network
- CMake 3.21+
- Xcode Command Line Tools

## Сборка

    chmod +x build_macos.sh deploy_macos.sh
    ./build_macos.sh

## Создание приложения .app

    ./deploy_macos.sh

Результат:

    build/SleepGuard.app

Для распространения через Gatekeeper потребуется code signing и, при
необходимости, notarization от Apple.
