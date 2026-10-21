
#include "Timer.h"
#include <chrono>

using Horloge = std::chrono::steady_clock;

void Timer::demarrer() {
    debutNanosecondes =
        std::chrono::duration_cast<std::chrono::nanoseconds>(
            Horloge::now().time_since_epoch()
        ).count();
}

double Timer::ecouleMicrosecondes() const {
    long long maintenant =
        std::chrono::duration_cast<std::chrono::nanoseconds>(
            Horloge::now().time_since_epoch()
        ).count();

    // Conversion des nanosecondes en microsecondes.
    return (maintenant - debutNanosecondes) / 1000.0;
}
