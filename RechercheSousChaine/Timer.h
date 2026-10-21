
#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

#include <chrono>

class Timer {
public:
    Timer();

    void start();
    void stop();

    double elapsedMicroseconds() const;
    double elapsedMilliseconds() const;
    double elapsedSeconds() const;

private:
    using Clock = std::chrono::steady_clock;

    Clock::time_point m_start;
    Clock::time_point m_end;
    bool m_running;
};

#endif // TIMER_H_INCLUDED
