#pragma once
#include <QDateTime>
#include <QVector>

namespace SleepGuard {

enum class SleepStage { Unknown, Awake, Light, Deep, REM };

struct Epoch {
    QDateTime timestamp;
    double heartRate = 0.0;
    double rmssd = 0.0;
    double movement = 0.0;
    double respiration = 0.0;
    SleepStage stage = SleepStage::Unknown;
};

struct SleepReport {
    QDateTime start;
    QDateTime end;
    int totalMinutes = 0;
    int awakeMinutes = 0;
    int lightMinutes = 0;
    int deepMinutes = 0;
    int remMinutes = 0;
    double averageHeartRate = 0.0;
    int quality = 0;
    QVector<Epoch> epochs;
};

QString stageName(SleepStage stage);

} // namespace SleepGuard
