
#include "Timer.h"

Timer::Timer() : m_running(false) {}

void Timer::start() {
    m_start = Clock::now();
    m_end = m_start;
    m_running = true;
}

void Timer::stop() {
    if (m_running) {
        m_end = Clock::now();
        m_running = false;
    }
}

double Timer::elapsedMicroseconds() const {
    const auto end = m_running ? Clock::now() : m_end;

    return std::chrono::duration<double, std::micro>(
        end - m_start
    ).count();
}

double Timer::elapsedMilliseconds() const {
    const auto end = m_running ? Clock::now() : m_end;

    return std::chrono::duration<double, std::milli>(
        end - m_start
    ).count();
}

double Timer::elapsedSeconds() const {
    const auto end = m_running ? Clock::now() : m_end;

    return std::chrono::duration<double>(
        end - m_start
    ).count();
}
