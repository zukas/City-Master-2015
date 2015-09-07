#ifndef GBUFFER_H
#define GBUFFER_H

#include "types.h"

class GBuffer {

  private:
	uint32_t m_frame_buffer{0};
	uint32_t m_position_buffer{0};
	uint32_t m_normal_buffer{0};
	uint32_t m_colour_buffer{0};
	uint32_t m_depth_buffer{0};

  public:
	GBuffer() = default;
	~GBuffer();

	void init();
	void clear();

	void begin_geom_pass();
	void end_geom_pass();

	void begin_render_pass();
	void end_render_pass();

};

#endif // GBUFFER_H
