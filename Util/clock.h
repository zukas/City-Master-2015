#ifndef CLOCK_H
#define CLOCK_H

#include <stdint.h>

class Clock
{
    private:
		static float m_time;

    public:
        Clock() = delete;

        static uint64_t now();
        static void update();
		static float getDuration();
};

#endif // CLOCK_H
