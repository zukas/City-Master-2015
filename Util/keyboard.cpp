#include "keyboard.h"
#include "clock.h"

Keyboard::Keyboard(GLFWwindow *window) : m_window(window) {}

Keyboard::~Keyboard() {}

void Keyboard::update() {
    static float lastTime = Clock::now();
    float currentTime = Clock::now();
    float deltaTime = (currentTime - lastTime) / 1000000000.f;
    lastTime = currentTime;

    if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) {
        m_events[0](-deltaTime);
    } else if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS) {
        m_events[0](deltaTime);
    }

    if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) {
        m_events[1](-deltaTime);
    } else if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS) {
        m_events[1](deltaTime);
    }

    if (glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS) {
        m_events[2](deltaTime);
    } else if (glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        m_events[2](-deltaTime);
    }

    if (glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        m_events[3](-deltaTime);
    } else if (glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        m_events[3](deltaTime);
    }

    if (glfwGetKey(m_window, GLFW_KEY_PAGE_UP) == GLFW_PRESS) {
        m_events[4](deltaTime);
    } else if (glfwGetKey(m_window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) {
        m_events[4](-deltaTime);
    }
}

void Keyboard::rotateHorizontal(KeyboardEvent e) { m_events[0] = std::move(e); }

void Keyboard::rotateVertical(KeyboardEvent e) { m_events[1] = std::move(e); }

void Keyboard::moveX(KeyboardEvent e) { m_events[2] = std::move(e); }

void Keyboard::moveY(KeyboardEvent e) { m_events[3] = std::move(e); }

void Keyboard::moveZ(KeyboardEvent e) { m_events[4] = std::move(e); }
