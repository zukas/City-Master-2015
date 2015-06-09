#include "mouse.h"
#include "gldebugger.h"
#include <cmath>

constexpr float epsilon { 25.f };
constexpr float scaler { 750.f };


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
        if(glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
            double x { 0 }, y { 0 };
            glfwGetCursorPos(m_window, &x, &y);
            if (m_rDrag == false)
            {
                m_pressPosX = m_currPosX = x;
                m_pressPosY = m_currPosY = y;
                m_rDrag = true;
                m_lClick = true;
            }
            else
            {

                m_currPosX = x;
                m_currPosY = y;

                float diff = m_currPosX - m_pressPosX;
                float absDiff = fabs(diff);
				LOG("Left button move X - diff: %f, log: %f", diff, log(abs(diff)));
				float diffScale = scaler*diff/absDiff;
				if (abs(diff) > epsilon && m_eventsDrag[0])
                {
                    m_lClick = false;
					m_eventsDrag[0](MOVE_X, std::log2(absDiff)/diffScale);
                }
                diff = m_pressPosY - m_currPosY;
                absDiff = fabs(diff);
				diffScale = scaler*diff/absDiff;
				if (abs(diff) > epsilon && m_eventsDrag[0])
                {
                    m_lClick = false;
					m_eventsDrag[0](MOVE_Y, std::log2(absDiff)/diffScale);
                }
            }
        }
        else if((m_currPosX || m_currPosY) && glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
        {
            double x { 0 }, y { 0 };
            glfwGetCursorPos(m_window, &x, &y);

            if (m_lClick)
            {
                if (m_eventsClick[0])
                {
                    m_eventsClick[0](m_currPosX, m_currPosY);
                }
                if (m_eventsClick[1])
                {
                    m_eventsClick[1](x, y);
                }
                if(m_eventsClick[2] && std::fabs(x - m_currPosX) < epsilon && std::fabs(y - m_currPosY) < epsilon)
                {
                    m_eventsClick[2](x, y);
                }
            }

            m_currPosX = m_currPosY = 0;
        }
        else if(glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS )
        {

            double x { 0 }, y { 0 };
            glfwGetCursorPos(m_window, &x, &y);
            if (m_rDrag == false)
            {
                m_pressPosX = m_currPosX = x;
                m_pressPosY = m_currPosY = y;
                m_rDrag = true;
            }
            else
            {
                m_currPosX = x;
                m_currPosY = y;

                float diff = m_currPosX - m_pressPosX;
                float absDiff = fabs(diff);
				LOG("Right button move X - diff: %f, log: %f", diff, log(abs(diff)));
				float diffScale = scaler*diff/absDiff;
				if (abs(diff) > epsilon)
                {
					m_eventsDrag[1](MOVE_X, -std::log2(absDiff)/diffScale);
                }
                diff = m_pressPosY - m_currPosY;
                absDiff = fabs(diff);
				diffScale = scaler*diff/absDiff;
				if (abs(diff) > epsilon)
                {
					m_eventsDrag[1](MOVE_Y, -std::log2(absDiff)/diffScale);
                }
            }
        }
        else if(glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE )
        {
            if (m_rDrag == true)
            {
                m_pressPosX = m_pressPosY = 0;
                m_currPosX = m_currPosY = 0;
                m_rDrag = false;
            }
        }
    }
}


void Mouse::onPressLeft(MouseClickEvent e)
{
    m_eventsClick[0] = std::move(e);
}

void Mouse::onReleaseLeft(MouseClickEvent e)
{
    m_eventsClick[1] = std::move(e);
}

void Mouse::onClickLeft(MouseClickEvent e)
{
    m_eventsClick[2] = std::move(e);
}

void Mouse::onDragLeft(MouseDragEvent e)
{
    m_eventsDrag[0] = std::move(e);
}

void Mouse::onDragRight(MouseDragEvent e)
{
    m_eventsDrag[1] = std::move(e);
}
