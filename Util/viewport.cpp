#include "viewport.h"
#include <glm/gtc/matrix_transform.hpp>

rect Viewport::g_viewport{0.f, 0.f, 0.f, 0.f};
glm::mat4 Viewport::g_ortho{};

void Viewport::update(const rect &viewport_) {
	g_viewport = viewport_;
	g_ortho = glm::ortho(viewport_.x, viewport_.w, viewport_.y, viewport_.h);
}

const rect &Viewport::viewport() { return g_viewport; }

const glm::mat4 &Viewport::ortho() { return g_ortho; }
