#ifndef GBUFFER_H
#define GBUFFER_H

#include "types.h"

class GBuffer {

  private:
	struct {
		uint32_t frame_buffer{0};
		uint32_t position_buffer{0};
		uint32_t normal_buffer{0};
		uint32_t colour_buffer{0};
		uint32_t depth_buffer{0};
	} m_multisample;
	struct {
		uint32_t frame_buffer{0};
		uint32_t position_buffer{0};
		uint32_t normal_buffer{0};
		uint32_t colour_buffer{0};
	} m_final;

	uint32_t m_sampler;

  public:
	GBuffer() = default;
	~GBuffer();

	void init();
	void clear();

	void begin_geom_pass();
	void end_geom_pass();

	void begin_render_pass();
	void end_render_pass();

	uint32_t colour_buffer_id();
};

class GBuffer2 {
	private:
		uint32_t m_frame_buffer;
		uint32_t m_position_buffer;
		uint32_t m_normal_buffer;
		uint32_t m_colour_buffer;
		uint32_t m_depth_buffer;


	public:
		GBuffer2() = default;

		void init(uint32_t width, uint32_t height, uint32_t samples);
		void clear();

		void start_frame();
		void geom_pass();
		void stencli_pass();
		void light_pass();

};

#endif // GBUFFER_H
