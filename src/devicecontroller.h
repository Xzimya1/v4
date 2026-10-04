#pragma once
#include <QObject>
#include <QTimer>
#include <QVector>
#include "sleepdata.h"

namespace SleepGuard {

// Абстракция устройств для MVP.
// Сейчас работает в DEMO-режиме.
// Следующий слой подключает QBluetoothDeviceDiscoveryAgent,
// QLowEnergyController и реальный BLE GATT-профиль браслета.
class DeviceController : public QObject {
    Q_OBJECT
public:
    explicit DeviceController(QObject* parent = nullptr);

    void startDemoStream();
    void stopDemoStream();
    void setLampAlarm(bool enabled);
    bool lampAlarmEnabled() const;

signals:
    void epochReceived(const SleepGuard::Epoch& epoch);
    void heartRateChanged(double bpm);
    void lampStateChanged(bool on);
    void deviceStatusChanged(const QString& status);

private:
    QTimer m_timer;
    bool m_lampAlarm = false;
    int m_counter = 0;
};

} // namespace SleepGuard
