#include "mouse.h"
#include <cmath>
#include <iostream>

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
        static double lastTime = glfwGetTime();
        double currentTime = glfwGetTime();
        float deltaTime = float(currentTime - lastTime);
        if(m_currPosX == 0 && m_currPosY == 0 && glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
            glfwGetCursorPos(m_window, &m_currPosX, &m_currPosY);
            if(m_eventsLeft[0])
            {
                m_eventsLeft[0](m_currPosX, m_currPosY);
            }

        }
        else if((m_currPosX || m_currPosY) && glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
        {
            double x { 0 }, y { 0 };
            glfwGetCursorPos(m_window, &x, &y);

            if(m_eventsLeft[1])
            {
                m_eventsLeft[1](x, y);
            }

            if(m_eventsLeft[2] && std::fabs(x - m_currPosX) < epsilon && std::fabs(y - m_currPosY) < epsilon)
            {
                m_eventsLeft[2](x, y);
            }

            m_currPosX = m_currPosY = 0;
        }
        else if(glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS )
        {

            double x { 0 }, y { 0 };
            glfwGetCursorPos(m_window, &x, &y);
            if (m_drag == false)
            {
                m_pressPosX = m_currPosX = x;
                m_pressPosY = m_currPosY = y;
                m_drag = true;
            }
            else
            {
                m_currPosX = x;
                m_currPosY = y;

                float diff = m_currPosX - m_pressPosX;
                float absDiff = fabs(diff);
                std::cout << diff << " " << log(abs(diff)) << std::endl;
                float diffScale = 250.f*diff/absDiff;
                if (abs(diff) > 10)
                {
                    m_eventsRight[0](MOVE_X, log(absDiff)/diffScale);
                }
                diff = m_pressPosY - m_currPosY;
                absDiff = fabs(diff);
                diffScale = 250.f*diff/absDiff;
                if (abs(diff) > 10)
                {
                    m_eventsRight[0](MOVE_Y, log(absDiff)/diffScale);
                }
            }
        }

    }
    else if(glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE )
    {
        if (m_drag == true)
        {
            m_pressPosX = m_pressPosY = 0;
            m_currPosX = m_currPosY = 0;
            m_drag = false;
        }
    }
}

void Mouse::onPressLeft(MouseLeftEvent e)
{
    m_eventsLeft[0] = std::move(e);
}

void Mouse::onReleaseLeft(MouseLeftEvent e)
{
    m_eventsLeft[1] = std::move(e);
}

void Mouse::onclickLeft(MouseLeftEvent e)
{
    m_eventsLeft[2] = std::move(e);
}

void Mouse::onDragRight(MouseRightEvent e)
{
    m_eventsRight[0] = std::move(e);
}
