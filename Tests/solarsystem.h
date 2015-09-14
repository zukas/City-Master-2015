#ifndef SOLARSYSTEM_H
#define SOLARSYSTEM_H

#include "Util/types.h"

struct model_phx_data_t {
	float distance;
    float rotation_speed;
    float orbital_speed;
    float axis_tilt;
    uint16_t children_count;
};

struct model_gfx_data_t {
    uint32_t texture;
    uint32_t mesh;
    uint32_t elem_count;
};

struct model_bin_data_t {
    byte *texture_data;
    uv_3d_vertex *model_data;
    uint32_t *index_data;

    uint32_t texture_size;
    uint32_t model_size;
    uint32_t index_size;
};

class SolarSystem {

  private:
    static uint32_t solar_model_id;
    void *m_memory;
    model_phx_data_t *m_model_phx_data;
    glm::mat4 *m_model_matrixes;
    model_gfx_data_t *m_model_gfx_data;
    uint16_t m_size;

  public:
    SolarSystem();
    void init(uint16_t m_size, uint32_t model_id);
    void deinit();
    void set(uint16_t index, const model_phx_data_t &model_phx,
             const model_bin_data_t &model_bin);

    void prepare();
    void render();
    void render_selection();
};

struct solar_program_t {
    uint32_t program_id;
    uint32_t projection_id;
    uint32_t view_id;
    uint32_t model_id;
};

#endif // SOLARSYSTEM_H
