#ifndef CLOCK_H
#define CLOCK_H


class Clock
{
    private:
		static float m_time;

    public:
        Clock() = delete;

        static void update();
		static float getDuration();
};

#endif // CLOCK_H
