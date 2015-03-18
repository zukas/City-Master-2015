#include "mouse.h"
#include <cmath>

constexpr int epsilon { 25 };

Mouse::Mouse(GLFWwindow *window) :
    m_window(window)
{
}

Mouse::~Mouse()
{

}

void Mouse::update()
{
    if(m_window)
    {
        if(m_posX == 0 && m_posY == 0 && glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
            glfwGetCursorPos(m_window, &m_posX, &m_posY);
            if(m_events[0])
            {
                m_events[0](m_posX, m_posY);
            }

        }
        else if((m_posX || m_posY) && glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
        {
            double x { 0 }, y { 0 };
            glfwGetCursorPos(m_window, &x, &y);

            if(m_events[1])
            {
                m_events[1](x, y);
            }

            if(m_events[2] && std::fabs(x - m_posX) < epsilon && std::fabs(y - m_posY) < epsilon)
            {
                m_events[2](x, y);
            }

            m_posX = m_posY = 0;
        }
    }
}

void Mouse::onPress(MouseEvent e)
{
    m_events[0] = std::move(e);
}

void Mouse::onRelease(MouseEvent e)
{
    m_events[1] = std::move(e);
}

void Mouse::onclick(MouseEvent e)
{
    m_events[2] = std::move(e);
}

