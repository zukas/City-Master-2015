#include "solarsystem.h"

#include "Util/clock.h"
#include "Util/program.h"
#include "Util/mesh3dcollection.h"
#include "Util/texture2dcollection.h"
#include "Util/profiler.h"

#include "Common/ptr_arithm.h"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

glm::mat4 moved_matrix(float dst, float angle = 0.f) {
	return glm::rotate(glm::mat4{{1.f, 0.f, 0.f, 0.f},
								 {0.f, 1.f, 0.f, 0.f},
								 {0.f, 0.f, 1.f, 0.f},
								 {dst, 0.f, 0.f, 1.f}},
					   angle, glm::vec3(0.f, 1.f, 0.f));
}

glm::quat rot_quat_y(float h_ang) {
    return glm::quat{glm::cos(h_ang), 0.f, glm::sin(h_ang), 0.f};
}

glm::quat base_rot_y(float current, float val) {
    return val != 0 ? rot_quat_y(PI * (current / val)) : glm::quat();
}

glm::quat rot_quat_z(float h_ang) {
    return glm::quat{glm::cos(h_ang), 0.f, 0.f, glm::sin(h_ang)};
}
uint16_t calculate_matrixes(const glm::mat4 &parent_model,
                            const model_phx_data_t *model_phx_data,
							glm::mat4 *model_matrixes, uint32_t idx = 0) {
    PROF("Preparing solar object");
    uint16_t process_count = 1;
    uint16_t children_count;
    glm::mat4 base_matrix;
    {
        PROF("Calculating solar object matrix");
		float current = Clock::getDuration();
		//		current = current + (current * (idx + 1) * 100 * PI2 /
		// 30);
        const model_phx_data_t phx_data = model_phx_data[0];
        children_count = phx_data.children_count;

		const float offset_angle = (float(idx + 1) * PI2 / 30.f) * 9050000.f;
		current += offset_angle;
		const glm::quat l_rot = base_rot_y(current, phx_data.rotation_speed);
		const glm::quat o_rot = base_rot_y(current, phx_data.orbital_speed);
        const glm::quat o_rot_i = -o_rot;

        base_matrix =
			glm::mat4_cast(o_rot) * moved_matrix(phx_data.distance) *
            glm::mat4_cast(
                o_rot_i * glm::quat_cast(glm::eulerAngleZ(phx_data.axis_tilt)));
        model_matrixes[0] = parent_model * base_matrix * glm::mat4_cast(l_rot);
    }

    if (children_count != 0) {
        base_matrix = parent_model * base_matrix;
        for (; children_count != 0; --children_count) {
			process_count += calculate_matrixes(
				base_matrix, &model_phx_data[process_count],
				&model_matrixes[process_count], process_count + idx);
        }
    }

    return process_count;
}

uint32_t SolarSystem::solar_model_id{0};

SolarSystem::SolarSystem()
    : m_memory(nullptr), m_model_phx_data(nullptr), m_model_matrixes(nullptr),
      m_model_gfx_data(nullptr) {}

void SolarSystem::init(uint16_t size, uint32_t model_id) {
    deinit();
    solar_model_id = model_id;
    m_memory = nullptr;
    m_model_phx_data = nullptr;
    m_model_matrixes = nullptr;
    m_model_gfx_data = nullptr;

    m_memory = malloc((sizeof(model_phx_data_t) + sizeof(model_gfx_data_t) +
                       sizeof(glm::mat4)) *
                      size);
    m_model_phx_data = (model_phx_data_t *)m_memory;
    m_model_matrixes =
        (glm::mat4 *)ptr_add(m_memory, sizeof(model_phx_data_t) * size);
    m_model_gfx_data = (model_gfx_data_t *)ptr_add(
        m_memory, (sizeof(model_phx_data_t) + sizeof(glm::mat4)) * size);
    m_size = size;

	m_sampler = Texture2DCollection::create_sampler();
	glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(m_sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(m_sampler, GL_TEXTURE_MIN_FILTER,
						GL_LINEAR_MIPMAP_LINEAR);
	//	glSamplerParameteri(m_sampler, GL_TEXTURE_MIN_LOD , -100000);
	//	glSamplerParameteri(m_sampler, GL_TEXTURE_MAX_LOD, 100000);
}

void SolarSystem::deinit() {
    // REMEMBER TO DEINIT TEXTURES AND MESH VERTEX ARRAYS
    free(m_memory);
    m_size = 0;
}

void SolarSystem::set(uint16_t index, const model_phx_data_t &model_phx,
                      const model_bin_data_t &model_bin) {

    m_model_phx_data[index] = model_phx;
	m_model_matrixes[index] = moved_matrix(model_phx.distance);

    model_gfx_data_t gfx_data{
		Texture2DCollection::create_jpg_from_memory(model_bin.texture_data,
													model_bin.texture_size),
        Mesh3DCollection::create(model_bin.model_data, model_bin.model_size,
                                 model_bin.index_data, model_bin.index_size),
        model_bin.index_size};

    m_model_gfx_data[index] = gfx_data;
}

void SolarSystem::prepare() {
	uint16_t processed =
		calculate_matrixes(glm::mat4(1.f), m_model_phx_data, m_model_matrixes);
	ASSERT(processed == m_size);
}

void SolarSystem::render() {

    const glm::mat4 *model_matrixes = m_model_matrixes;
    const model_gfx_data_t *model_gfx_data = m_model_gfx_data;
	Texture2DCollection::bind_sampler(m_sampler);
	for (uint16_t i = 0; i < m_size; ++i) {
        Program::set_uniform(solar_model_id, model_matrixes[i]);
        const model_gfx_data_t tmp = model_gfx_data[i];
        Texture2DCollection::bind(tmp.texture);
        Mesh3DCollection::render_geometry(tmp.mesh, tmp.elem_count);
        Texture2DCollection::unbind();
    }
	Texture2DCollection::unbind_sampler();
}

void SolarSystem::render_selection() {}
