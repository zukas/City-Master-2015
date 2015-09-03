#include "controlinput.h"
#include "clock.h"
#include <GLFW/glfw3.h>

ControlInput::ControlInput() {}

control ControlInput::poll(GLFWwindow *window) {

    static uint64_t start = Clock::now();

    control_value _nav = NONE;
    float _delta = float(Clock::now() - start) / 1000000000.f;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        _nav |= ROT_X_P;
    } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        _nav |= ROT_X_P;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        _nav |= ROT_Y_P;
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        _nav |= ROT_Y_N;
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        _nav |= MOVE_X_P;
    } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        _nav |= MOVE_X_N;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        _nav |= MOVE_Y_N;
    } else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        _nav |= MOVE_Y_P;
    }

    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS) {
        _nav |= ZOOM_P;
    } else if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) {
        _nav |= ZOOM_N;
    }

    start = Clock::now();
    return control{_nav, _delta};
}
