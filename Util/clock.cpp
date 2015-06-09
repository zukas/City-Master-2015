#include "clock.h"
#include <chrono>

float Clock::m_time { 0.0 };

void Clock::update()
{
    static auto first = std::chrono::high_resolution_clock::now();
    m_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - first).count();
}

float Clock::getDuration()
{
    return m_time;
}
