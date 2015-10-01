#include "mouse.h"

#include <GLFW/glfw3.h>

float Mouse::x_pos { -1.f };
float Mouse::y_pos { -1.f };

void Mouse::update(GLFWwindow *window)
{
	double x { -1.f };
	double y { -1.f };
	glfwGetCursorPos(window, &x, &y);
	x_pos = x;
	y_pos = y;
}

float Mouse::x()
{
	return x_pos;
}

float Mouse::y()
{
	return y_pos;
}

