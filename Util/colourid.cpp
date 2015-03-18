#include "colourid.h"
#include <algorithm>

std::vector<int > ColourID::g_colourID {};

ColourID::ColourID(int id) :
    m_value(id),
    m_red(id & ~(max_green | max_blue)),
    m_green((id & ~ (max_red | max_blue)) >> 8),
    m_blue((id & ~ (max_red | max_green) ) >> 16),
    m_colour(m_red / 255.f, m_green / 255.f, m_blue / 255.f, 1.f)
{ 
}

ColourID::ColourID(int in_red, int in_green, int in_blue) :
    m_value(in_red | (in_green << 8) | (in_blue << 16)),
    m_red(in_red),
    m_green(in_green),
    m_blue(in_blue),
    m_colour(m_red / 255.f, m_green / 255.f, m_blue / 255.f, 1.f)
{
}

int ColourID::value() const
{
    return m_value;
}

const float *ColourID::colour() const
{
    return &m_colour[0];
}

int ColourID::red() const
{
    return m_red;
}

int ColourID::green() const
{
    return m_green;
}

int ColourID::blue() const
{
    return m_blue;
}


ColourID::operator glm::vec4() const
{
    return m_colour;
}

ColourID::operator int() const
{
    return m_value;
}

ColourID ColourID::aquireID()
{
    int id = 0;

    auto it = std::find(std::begin(g_colourID), std::end(g_colourID), 0);
    if(it == std::end(g_colourID))
    {
        g_colourID.push_back(g_colourID.empty() ? id = 1 : id = g_colourID.back() + 1);
    }
    else if(it == std::begin(g_colourID))
    {
        id = *it = 1;
    }
    else
    {
        id = *it = *(it - 1) + 1;
    }

    return id;
}

void ColourID::releaseID(int id)
{
    if(id > 0)
    {
        auto it = std::find(std::begin(g_colourID), std::end(g_colourID), id);
        if(it != std::end(g_colourID))
        {
            *it = 0;
        }
    }
}

