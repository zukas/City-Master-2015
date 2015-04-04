#include "lamp.h"

Lamp::Lamp(glm::vec3 postion, float strength) :
    m_position(std::move(postion)),
    m_strength(strength)
{

}

void Lamp::update(Program &program)
{
    program.setLightStrength(m_strength);
    program.setLigthPosition(m_position);
}
