#include "devicecontroller.h"
#include "sleepengine.h"
#include <QRandomGenerator>
#include <QtMath>

namespace SleepGuard {

DeviceController::DeviceController(QObject* parent) : QObject(parent) {
    connect(&m_timer, &QTimer::timeout, this, [this]() {
        Epoch e;
        e.timestamp = QDateTime::currentDateTime();
        e.heartRate = 52.0 + QRandomGenerator::global()->bounded(18.0);
        e.rmssd = 25.0 + QRandomGenerator::global()->bounded(45.0);
        e.movement = QRandomGenerator::global()->generateDouble();
        e.respiration = 11.0 + QRandomGenerator::global()->bounded(5.0);
        e.stage = SleepEngine::estimateStage(e);

        emit epochReceived(e);
        emit heartRateChanged(e.heartRate);
        ++m_counter;
    });
}

void DeviceController::startDemoStream() {
    m_counter = 0;
    m_timer.start(1000); // demo: одна "30-секундная эпоха" каждую секунду
    emit deviceStatusChanged("DEMO: браслет подключён");
}

void DeviceController::stopDemoStream() {
    m_timer.stop();
    emit deviceStatusChanged("Браслет отключён");
}

void DeviceController::setLampAlarm(bool enabled) {
    m_lampAlarm = enabled;
    emit lampStateChanged(enabled);
}

bool DeviceController::lampAlarmEnabled() const {
    return m_lampAlarm;
}

} // namespace SleepGuard
