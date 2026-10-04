#pragma once
#include "sleepdata.h"

namespace SleepGuard {

// MVP baseline. Это НЕ клиническая классификация сна.
// В реальном продукте сюда подключается обученная модель
// LightGBM/XGBoost после валидации на эталонных данных.
class SleepEngine {
public:
    static SleepStage estimateStage(const Epoch& e);
    static SleepReport makeReport(const QVector<Epoch>& epochs);
    static bool isLightSleep(const Epoch& e);

    // Возвращает true, если в текущий момент можно запускать будильник.
    static bool shouldWake(const QVector<Epoch>& recentEpochs,
                           const QTime& now,
                           const QTime& windowStart,
                           const QTime& windowEnd);
};

} // namespace SleepGuard
