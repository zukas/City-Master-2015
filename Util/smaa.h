#ifndef SMAA_H
#define SMAA_H

#include "types.h"

class SMAA {
  private:
    uint32_t m_programs[3];
    uint32_t m_frame_buffers[3];
    uint32_t m_texture_buffers[3];
    uint32_t m_data_buffers[2];

  public:
    SMAA() = default;
    void init(const char *shader_source, byte *area, byte *search);
    void run(uint32_t src_texture_buffer, uint32_t dest_frame_buffer = 0);

  private:
    void enge_detection_pass();
    void blend_weight_pass();
    void neighbor_blend_pass();
};

#endif // SMAA_H
