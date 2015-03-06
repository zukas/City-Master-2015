#ifndef CLOCK_H
#define CLOCK_H


class Clock
{
    private:
        static double m_time;

    public:
        Clock() = delete;

        static void update();
        static double getDuration();
};

#endif // CLOCK_H
