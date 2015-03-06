#include "lamp.h"

Lamp::Lamp(glm::vec3 postion, float strength) :
    m_position(std::move(postion)),
    m_strength(strength)
{

}

void Lamp::update(Program &program)
{
    program.setUniform(glsl_light_position, m_position);
    program.setUniform(glsl_light_strength, m_strength);
}
