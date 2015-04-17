#include "keyboard.h"
#include <iostream>


Keyboard::Keyboard(GLFWwindow *window):
m_window(window)
{
}

Keyboard::~Keyboard()
{

}

void Keyboard::update()
{
    if(m_window)
    {
        static double lastTime = glfwGetTime();
        double currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        if (glfwGetKey( m_window, GLFW_KEY_D) == GLFW_PRESS)
        {
            m_events[0](-deltaTime);
        }
        else if (glfwGetKey( m_window, GLFW_KEY_A) == GLFW_PRESS)
        {
            m_events[0](deltaTime);
        }
        else if (glfwGetKey( m_window, GLFW_KEY_UP ) == GLFW_PRESS)
        {
            m_events[1](deltaTime);
        }
        else if (glfwGetKey( m_window, GLFW_KEY_DOWN ) == GLFW_PRESS)
        {
            m_events[1](-deltaTime);
        }
        else if (glfwGetKey( m_window, GLFW_KEY_LEFT ) == GLFW_PRESS)
        {
            m_events[2](-deltaTime);
        }
        else if (glfwGetKey( m_window, GLFW_KEY_RIGHT ) == GLFW_PRESS)
        {
            m_events[2](deltaTime);
        }
        else if(glfwGetKey(m_window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
        {
            std::cout << deltaTime << std::endl;
            m_events[3](deltaTime*100);
        }
        else if(glfwGetKey(m_window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
        {
            m_events[3](-deltaTime);
        }
        lastTime = currentTime;
    }
}

void Keyboard::rotate(KeyboardEvent e)
{
    m_events[0] = std::move(e);
}

void Keyboard::moveX(KeyboardEvent e)
{
    m_events[1] = std::move(e);
}

void Keyboard::moveY(KeyboardEvent e)
{
    m_events[2] = std::move(e);
}

void Keyboard::moveZ(KeyboardEvent e)
{
    m_events[3] = std::move(e);
}
