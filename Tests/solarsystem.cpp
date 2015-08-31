#include "solarsystem.h"
#include "Memory/utils.h"

#include "Util/clock.h"
#include "Util/program.h"
#include "Util/mesh3dcollection.h"
#include "Util/texture2dcollection.h"
#include "Util/profiler.h"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

constexpr float year{5 * 60 * 1000.f};
constexpr float size_div{6371.f};
constexpr float distance_div{14959.78707f};
constexpr float rat{distance_div / size_div};
constexpr float days_in_year{365.256f};
constexpr float day{year / days_in_year};
constexpr float hour{day / 23.9344722f};
constexpr float degree{0.0174532925f};

uint16_t calculate_matrixes(const glm::mat4 &parent_model,
							const model_phx_data_t *model_phx_data,
							glm::mat4 *model_matrixes) {
	PROF("Calculating solar object matrixes and processing children");
	uint16_t process_count = 1;
	uint16_t children_count;
	glm::mat4 base_matrix;
	{
		PROF("Calculating solar object matrixes");
		float current = Clock::getDuration();
		const model_phx_data_t phx_data = model_phx_data[0];
		children_count = phx_data.children_count;

		const glm::mat4 l_rot =
			phx_data.rotation_speed > 0.f
				? glm::mat4_cast(
					  glm::angleAxis(PI2 * (current / phx_data.rotation_speed),
									 glm::vec3(0.f, 1.f, 0.f)))
				: glm::mat4(1.f);
		const glm::mat4 o_rot =
			phx_data.orbital_speed > 0.f
				? glm::mat4_cast(
					  glm::angleAxis(PI2 * (current / phx_data.orbital_speed),
									 glm::vec3(0.f, 1.f, 0.f)))
				: glm::mat4(1.f);

		const glm::mat4 mat = glm::translate(glm::mat4(1.f), phx_data.location);

		const glm::mat4 tilt =
			glm::inverse(o_rot) * glm::eulerAngleYXZ(phx_data.axis_tilt.y,
													 phx_data.axis_tilt.x,
													 phx_data.axis_tilt.z);

		base_matrix = o_rot * mat * tilt;
		model_matrixes[0] = parent_model * base_matrix * l_rot;
	}
	if (children_count != 0) {
		base_matrix = parent_model * base_matrix;
		for (; children_count != 0; --children_count) {
			process_count +=
				calculate_matrixes(base_matrix, &model_phx_data[process_count],
								   &model_matrixes[process_count]);
		}
	}

	return process_count;
}

uint32_t SolarSystem::solar_model_id{0};

SolarSystem::SolarSystem() {}

void SolarSystem::init(uint16_t size, uint32_t model_id) {
	solar_model_id = model_id;
	m_memory = malloc((sizeof(model_phx_data_t) + sizeof(model_gfx_data_t) +
					   sizeof(glm::mat4)) *
					  size);
	m_model_phx_data = (model_phx_data_t *)m_memory;
	m_model_matrixes =
		(glm::mat4 *)ptr_add(m_memory, sizeof(model_phx_data_t) * size);
	m_model_gfx_data = (model_gfx_data_t *)ptr_add(
		m_memory, (sizeof(model_phx_data_t) + sizeof(glm::mat4)) * size);
	m_size = size;
}

void SolarSystem::deinit() {
	// REMEMBER TO DEINIT TEXTURES AND MESH VERTEX ARRAYS
	free(m_memory);
	m_size = 0;
}

void SolarSystem::set(uint16_t index, const model_phx_data_t &model_phx,
					  const model_bin_data_t &model_bin) {

	m_model_phx_data[index] = model_phx;
	m_model_matrixes[index] = glm::mat4(1.f);

	model_gfx_data_t gfx_data{
		Texture2DCollection::create_dss_from_memory(model_bin.texture_data,
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

	for (uint16_t i; i < m_size; ++i) {
		Uniforms::setUniform(solar_model_id, model_matrixes[i]);
		const model_gfx_data_t tmp = model_gfx_data[i];
		Texture2DCollection::bind(tmp.texture);
		Mesh3DCollection::render_geometry(tmp.mesh, tmp.elem_count);
	}
}
