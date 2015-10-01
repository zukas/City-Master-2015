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

    uint32_t m_sampler{0};
    uint32_t m_width{0};
    uint32_t m_height{0};

  public:
    GBuffer() = default;
    ~GBuffer();

    void init(uint32_t width, uint32_t height, uint32_t samples);
    void clear();

    void begin_geom_pass();
    void end_geom_pass();

    void begin_render_pass();
    void end_render_pass();
};

#endif // GBUFFER_H
