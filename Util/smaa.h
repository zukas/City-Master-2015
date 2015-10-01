#ifndef SMAA_H
#define SMAA_H

#include "types.h"

class SMAA {
  public:
	struct edge_detection_program_t {
		uint32_t program_id;
        uint32_t rm_matrix_id;
	};

	struct blend_weight_program_t {
		uint32_t program_id;
		uint32_t rm_matrix_id;
	};

	struct neighbor_blend_program_t {
		uint32_t program_id;
		uint32_t rm_matrix_id;
	};

  private:
	uint32_t m_mesh_id;
	edge_detection_program_t m_edge_detection_program;
	blend_weight_program_t m_blend_weight_program;
	neighbor_blend_program_t m_neighbor_blend_program;
	uint32_t m_frame_buffers[2];
	uint32_t m_texture_buffers[2];
    uint32_t m_data_buffers[2];
	uint32_t m_sampler;

  public:
    SMAA() = default;
    void init(const char *shader_source, byte *area, byte *search);
	void clear();
	void run(uint32_t src_texture_buffer, uint32_t dest_frame_buffer = 0) const;

  private:
	void edge_detection_pass(uint32_t colour_texture_id) const;
	void blend_weight_pass() const;
	void neighbor_blend_pass(uint32_t colour_texture_id, uint32_t dst_framebuffer_id) const;
	void run_pass() const;
};

#endif // SMAA_H
