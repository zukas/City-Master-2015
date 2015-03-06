#include "axis.h"

Axis::Axis()
{

}

Axis::Axis(float size)
{
    std::vector<glm::vec3 > axisData {
        {-size, 0.0f, 0.0f },
        { size, 0.0f, 0.0f },

        { 0.0f, -size, 0.0f },
        { 0.0f,  size, 0.0f },

        { 0.0f, 0.0f, -size },
        { 0.0f, 0.0f,  size }
    };

    std::vector<glm::vec3 > axisColors {
        { 1.f, 0.f, 0.f },
        { 1.f, 0.f, 0.f },
        { 0.f, 1.f, 0.f },
        { 0.f, 1.f, 0.f },
        { 0.f, 0.f, 1.f },
        { 0.f, 0.f, 1.f }
    };

    m_axis = { std::move(axisData), std::move(axisColors), Mesh::LINE };
}

Axis::Axis(Axis &&other) :
    m_axis(std::move(other.m_axis))
{

}

void Axis::render(Program &program)
{
    m_axis.render(program);
}

Axis &Axis::operator =(Axis &&other)
{
    m_axis = std::move(other.m_axis);
    return *this;
}
