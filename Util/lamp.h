#ifndef LAMP_H
#define LAMP_H

#include <glm/glm.hpp>

class Lamp
{
        glm::vec4 m_light;
    public:
        Lamp(const glm::vec3 &postion = {}, float strength = 100.f);
        const glm::vec4 &getLight() const;
};

#endif // LAMP_H
