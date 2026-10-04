#include "sleepengine.h"
#include <algorithm>
#include <cmath>

namespace SleepGuard {

SleepStage SleepEngine::estimateStage(const Epoch& e) {
    // Простейший демонстрационный baseline.
    // Реальная модель должна обучаться на размеченных PSG/валидированных данных.
    if (e.movement > 0.70 || e.heartRate > 95.0)
        return SleepStage::Awake;

    if (e.movement < 0.12 && e.rmssd > 55.0 && e.heartRate < 58.0)
        return SleepStage::Deep;

    if (e.rmssd < 28.0 && e.movement < 0.30)
        return SleepStage::REM;

    return SleepStage::Light;
}

bool SleepEngine::isLightSleep(const Epoch& e) {
    return e.stage == SleepStage::Light;
}

SleepReport SleepEngine::makeReport(const QVector<Epoch>& epochs) {
    SleepReport r;
    r.epochs = epochs;
    if (epochs.isEmpty()) return r;

    r.start = epochs.first().timestamp;
    r.end = epochs.last().timestamp.addSecs(30);
    r.totalMinutes = static_cast<int>(epochs.size() * 0.5);

    double hrSum = 0.0;
    int hrCount = 0;

    for (const auto& e : epochs) {
        switch (e.stage) {
        case SleepStage::Awake: r.awakeMinutes += 0.5; break;
        case SleepStage::Light: r.lightMinutes += 0.5; break;
        case SleepStage::Deep:  r.deepMinutes += 0.5; break;
        case SleepStage::REM:   r.remMinutes += 0.5; break;
        default: break;
        }
        if (e.heartRate > 0) {
            hrSum += e.heartRate;
            ++hrCount;
        }
    }

    r.averageHeartRate = hrCount ? hrSum / hrCount : 0.0;

    const double sleepRatio =
        r.totalMinutes > 0
        ? 1.0 - static_cast<double>(r.awakeMinutes) / r.totalMinutes
        : 0.0;

    r.quality = std::clamp(static_cast<int>(sleepRatio * 100.0), 0, 100);
    return r;
}

bool SleepEngine::shouldWake(const QVector<Epoch>& recentEpochs,
                             const QTime& now,
                             const QTime& windowStart,
                             const QTime& windowEnd) {
    if (recentEpochs.isEmpty()) return false;
    if (now < windowStart || now > windowEnd) return false;

    // Для MVP достаточно последних нескольких эпох.
    // В production здесь должна работать валидированная модель стадий.
    const auto& latest = recentEpochs.last();
    return latest.stage == SleepStage::Light;
}

} // namespace SleepGuard
