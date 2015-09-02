#include "lamp.h"

Lamp::Lamp(const glm::vec3 &postion, float strength) :
    m_light(postion, strength)
{

}

const glm::vec4 &Lamp::getLight() const
{
    return m_light;
}

