#include "clock.h"
#include <chrono>

double Clock::m_time { 0.0 };

void Clock::update()
{
    static auto first = std::chrono::high_resolution_clock::now();
    m_time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - first).count() / 1000000000.00;
}

double Clock::getDuration()
{
    return m_time;
}
