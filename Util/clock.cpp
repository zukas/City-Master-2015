#include "clock.h"
#include <time.h>
#include <stdint.h>

float Clock::m_time{0.0};

uint64_t Clock::now() {
    struct timespec tp;
    clock_gettime(CLOCK_MONOTONIC_RAW, &tp);
    return tp.tv_sec * 1000000000 + tp.tv_nsec;
}

void Clock::update() {
    static uint64_t first = now();
    m_time = ((float)(now() - first)) / 1000000.f;
}

float Clock::getDuration() { return m_time; }
