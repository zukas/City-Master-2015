#ifndef AXIS_H
#define AXIS_H

#include "types.h"
#include "model.h"

class Axis
{
    private:
        Mesh m_axis;
    public:
        Axis();
        Axis(float size);
        Axis(Axis &&other);
        void render(Program &program);

        Axis &operator = (Axis &&other);

        Axis(const Axis&) = delete;
        Axis &operator = (const Axis&) = delete;
};

#endif // AXIS_H
