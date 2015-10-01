#include "controlinput.h"
#include "clock.h"
#include "mouse.h"
#include "viewport.h"

#include <GLFW/glfw3.h>

ControlInput::ControlInput() {}

control ControlInput::poll(GLFWwindow *window) {

    static uint64_t start = Clock::now();

    control res;

	float _delta = float(Clock::now() - start) / 2000000000.f;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        res.val |= ROT_X;
		res.delta[0] = -_delta;
    } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        res.val |= ROT_X;
		res.delta[0] = _delta;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        res.val |= ROT_Y;
		res.delta[1] = -_delta;
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        res.val |= ROT_Y;
		res.delta[1] = _delta;
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        res.val |= MOVE_X;
        res.delta[2] = _delta;
    } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        res.val |= MOVE_X;
        res.delta[2] = -_delta;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        res.val |= MOVE_Y;
        res.delta[3] = -_delta;
    } else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        res.val |= MOVE_Y;
        res.delta[3] = _delta;
    }

    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS) {
        res.val |= ZOOM;
        res.delta[4] = _delta;
    } else if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) {
        res.val |= ZOOM;
        res.delta[4] = -_delta;
    }

	float x = Mouse::x();
	float y = Mouse::y();

	if (y > 0 && !(res.val & MOVE_X)) {
		if (y < navigation_delta) {
			res.val |= MOVE_X;
			res.delta[2] = _delta;
		} else if (y > Viewport::height() - navigation_delta) {
			res.val |= MOVE_X;
			res.delta[2] = -_delta;
		}
	}

	if (x > 0 && !(res.val & MOVE_Y)) {
		if (x < navigation_delta) {
			res.val |= MOVE_Y;
			res.delta[3] = -_delta;
		} else if (x > Viewport::width() - navigation_delta) {
			res.val |= MOVE_Y;
			res.delta[3] = _delta;
		}
	}

    start = Clock::now();
    return res;
}
