#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "types.h"
#include <glm/glm.hpp>

class Viewport {

  private:
	static rect g_viewport;
	static glm::mat4 g_ortho;

  public:
	Viewport() = delete;
	static void update(const rect &viewport_);
	static const rect &viewport();
	static float width();
	static float height();
	static const glm::mat4 &ortho();
};

#endif // VIEWPORT_H
