#include "sleepdata.h"

namespace SleepGuard {

QString stageName(SleepStage stage) {
    switch (stage) {
    case SleepStage::Awake: return "Бодрствование";
    case SleepStage::Light: return "Лёгкий сон";
    case SleepStage::Deep:  return "Глубокий сон";
    case SleepStage::REM:   return "REM";
    default:                return "Не определено";
    }
}

} // namespace SleepGuard
