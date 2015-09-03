#include "controlinput.h"
#include "clock.h"
#include "viewport.h"

#include <GLFW/glfw3.h>

ControlInput::ControlInput() {}

control ControlInput::poll(GLFWwindow *window) {

    static uint64_t start = Clock::now();

    control res;
    res.val = NONE;

    double x{0}, y{0};
    glfwGetCursorPos(window, &x, &y);

    rect _viewport = Viewport::viewport();

    float _delta = float(Clock::now() - start) / 1000000000.f;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        res.val |= ROT_X;
        res.delta[0] = -_delta / 2.f;
    } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        res.val |= ROT_X;
        res.delta[0] = _delta / 2.f;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        res.val |= ROT_Y;
        res.delta[1] = -_delta / 2.f;
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        res.val |= ROT_Y;
        res.delta[1] = _delta / 2.f;
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        res.val |= MOVE_X;
        res.delta[2] = _delta;
    } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        res.val |= MOVE_X;
        res.delta[2] = -_delta;
    }

    if (!(res.val & MOVE_X)) {
        if (y < navigation_delta) {
            res.val |= MOVE_X;
            res.delta[2] = _delta;
        } else if (y > _viewport.h - navigation_delta) {
            res.val |= MOVE_X;
            res.delta[2] = -_delta;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        res.val |= MOVE_Y;
        res.delta[3] = -_delta;
    } else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        res.val |= MOVE_Y;
        res.delta[3] = _delta;
    }

    if (!(res.val & MOVE_Y)) {
        if (x < navigation_delta) {
            res.val |= MOVE_Y;
            res.delta[3] = -_delta;
        } else if (x > _viewport.w - navigation_delta) {
            res.val |= MOVE_Y;
            res.delta[3] = _delta;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS) {
        res.val |= ZOOM;
        res.delta[4] = _delta;
    } else if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) {
        res.val |= ZOOM;
        res.delta[4] = -_delta;
    }

    start = Clock::now();
    return res;
}
