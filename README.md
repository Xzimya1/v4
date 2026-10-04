# SleepGuard / Somni — C++ MVP

Это стартовый C++/Qt-проект по ТЗ SleepGuard v1.0.

## Что уже реализовано

- C++17 + Qt 6.
- Главный экран MVP.
- DEMO-поток данных браслета.
- Эпохи по 30 секунд.
- Пульс, RMSSD, движение, дыхание.
- Демонстрационная оценка стадий: Awake / Light / Deep / REM.
- Утренний отчёт.
- Расчёт простого показателя качества.
- Вилка пробуждения.
- Переключатель лампы-будильника.
- CSV-экспорт.
- Архитектурное разделение данных, алгоритма, устройств и UI.
- Qt Bluetooth и Network уже подключены в CMake для следующего этапа.

## Важно

Текущий `SleepEngine` — демонстрационный baseline, а не обученная медицинская модель.
По ТЗ реальная модель должна быть обучена и валидирована на эталонных данных:
PSG/валидированное устройство, разбиение по участникам, метрики accuracy/kappa/F1.

Также в DEMO-режиме одна эпоха моделируется каждую секунду, чтобы интерфейс было
удобно тестировать. В настоящем браслете эпоха составляет 30 секунд, а PPG
собирается непрерывно на частоте 25–100 Гц.

## Сборка

Требуется Qt 6 с модулями:
- Core
- Widgets
- Bluetooth
- Network

Пример:

    cmake -S . -B build
    cmake --build build

Затем запустить `SleepGuard`.

## Следующий этап: реальный браслет

Нужно определить GATT-профиль BLE, например:

- Service UUID: собственный UUID SleepGuard
- Characteristic PPG
- Characteristic ACC
- Characteristic battery
- Characteristic device status

После этого `DeviceController` заменяется на реализацию через
`QBluetoothDeviceDiscoveryAgent`, `QLowEnergyController` и
`QLowEnergyService`.

## Следующий этап: лампа ESP32

Приложение должно отправлять лампе:
- настройку будильника;
- начало рассвета;
- яркость;
- цветовую температуру/цвет;
- громкость;
- команду остановки будильника.

Для MVP можно использовать HTTPS REST или MQTT поверх Wi-Fi.

## Рекомендуемая production-архитектура

mobile C++/Qt
    -> BLE -> bracelet
    -> HTTPS/MQTT -> ESP32 lamp
    -> HTTPS REST -> backend
                       -> PostgreSQL/TimescaleDB
                       -> object storage
                       -> ML service

Сырые PPG хранить ограниченное время на телефоне, а в облако отправлять
30-секундные признаки и аудиофрагменты только при обнаруженных событиях.

## Автоматическая сборка macOS DMG

Для сборки без локального Qt используйте GitHub Actions. Workflow находится в
`.github/workflows/build-macos-dmg.yml` и собирает DMG отдельно для Apple Silicon
(arm64) и Intel (x86_64). Подробности — в `README_MACOS_GITHUB.md`.

## Automatic Intel macOS DMG

For an Intel Mac, use the GitHub Actions workflow in `.github/workflows/build-macos-dmg.yml`. It builds a native x86_64 DMG on `macos-15-intel`.
