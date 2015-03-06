#ifndef LAMP_H
#define LAMP_H

#include "types.h"
#include "program.h"
#include <glm/glm.hpp>

class Lamp
{
        glm::vec3 m_position;
        float m_strength;

    public:
        Lamp(glm::vec3 postion = {}, float strength = 100.f);
        void update(Program &program);
};

#endif // LAMP_H
