#include "glprogram.h"
#include "Memory/bitsmemory.h"
#include "Memory/utils.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <algorithm>

//#include "Import/loader.h"
#include "Util/clock.h"
#include "Util/shapes.h"
#include "Util/utils.h"
#include "Util/gldebugger.h"
#include "Util/profiler.h"
#include "Util/texture2d.h"
#include "Util/utils.h"
#include "Util/viewport.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_interpolation.hpp>
#include <glm/gtx/transform.hpp>

#include "resources/resourcemanager.h"
#include "Util/texture2dcollection.h"

#include <chrono>

constexpr float year{5 * 60 * 1000.f};
constexpr float size_div{6371.f};
constexpr float distance_div{14959.78707f};
constexpr float rat{distance_div / size_div};
constexpr float days_in_year{365.256f};
constexpr float day{year / days_in_year};
constexpr float hour{day / 23.9344722f};
constexpr float degree{0.0174532925f};

// glm::mat4 rot(const glm::mat4 &parent, )

struct planet_data {
    float orbiral_speed;
    float rotation_speed;
    glm::vec3 axis_tilt;
};

glm::mat4 __preproc_motion(const transform &t, model_data data) {

    float current = Clock::getDuration();
    planet_data *info = static_cast<planet_data *>(data);
    glm::mat4 mat = t;
    glm::mat4 rot = info->orbiral_speed > 0.f
                        ? glm::mat4_cast(glm::angleAxis(
                              PI2 * (current / info->orbiral_speed),
                              glm::vec3(0.f, 1.f, 0.f)))
                        : glm::mat4(1.f);
    glm::mat4 tilt = glm::inverse(rot) * glm::eulerAngleYXZ(info->axis_tilt.y,
                                                            info->axis_tilt.x,
                                                            info->axis_tilt.z);

    glm::mat4 res = rot * mat * tilt;
    return res;
}

glm::mat4 __postproc_motion(const glm::mat4 &t, model_data data) {
    float current = Clock::getDuration();
    planet_data *info = static_cast<planet_data *>(data);
    glm::mat4 res = t * (info->rotation_speed > 0.f
                             ? glm::mat4_cast(glm::angleAxis(
                                   PI2 * (current / info->rotation_speed),
                                   glm::vec3(0.f, 1.f, 0.f)))
                             : glm::mat4(1.f));
    return res;
}

float system_size{143.73f * pow(10, 9)};
float system_scale{system_size / 50000000.f};

float size_scaling(float amp, float x) {
    return std::sqrt((x / (amp * 2)) + std::sqrt(amp / 25));
}

float distance_scaling(float amp, float x) {
    return std::sqrt((x / (amp * 2)) + std::sqrt(amp / 25));
}

struct buffer {
    unsigned char *data;
    size_t size;
};

enum ObjectType { SUN, PLANET, MOON, SHALLOW_MOON };

class Planet {
  public:
    const char *name;
    float size;
    float distance;
    float orbital_period;
    float rotation_period;
    glm::vec3 axis_tilt;
    ObjectType type;
    resource res;

    float calc_size;
    float calc_distance;
    int children_count{0};
    Planet **children{nullptr};

  public:
    void setup(const char *_name, float _size, float _distance,
               float _orbtal_period, float _rotation_period,
               glm::vec3 _axis_tilt, ObjectType _type, resource _res);
    void setChildrenCount(int size);
    void setChild(int pos, Planet *chiled);
    void calc();
    Model generate(float parent_size = 0.f);
};

void Planet::setup(const char *_name, float _size, float _distance,
                   float _orbtal_period, float _rotation_period,
                   glm::vec3 _axis_tilt, ObjectType _type, resource _res) {
    name = _name;
    size = _size;
    distance = _distance;
    orbital_period = _orbtal_period;
    rotation_period = _rotation_period;
    axis_tilt = _axis_tilt;
    type = _type;
    res = _res;
}

void Planet::setChildrenCount(int size) {
    children_count = size;
    children = new Planet *[size];
}

void Planet::setChild(int pos, Planet *chiled) { children[pos] = chiled; }

void Planet::calc() {
    switch (type) {
    case SUN:
        calc_size = size_scaling(15.f, size);
        calc_distance =
            distance > 0.f ? distance_scaling(100000.f, distance) : 0.f;
        break;
    case PLANET:
        calc_size = size_scaling(40.f, size);
        calc_distance = distance_scaling(200.f, distance);
        break;
    case MOON:
    case SHALLOW_MOON:
        calc_size = size_scaling(255.f, size);
        calc_distance = distance_scaling(2000.f, distance);
        break;
    default:
        break;
    }
}

Model Planet::generate(float parent_size) {
    calc();
    float parts;
    switch (type) {
    case SUN:
        parts = 256;
        break;
    case PLANET:
        parts = 256;
        break;
    case MOON:
        parts = 128;
        break;
    case SHALLOW_MOON:
        parts = 16;
        break;
    default:
        parts = 4;
        break;
    }

    auto d = sphare::create(calc_size / 2.f, parts, parts);

    Mesh3D m{d.data.data(), d.data.size(), d.indexes.data(), d.indexes.size()};
    Texture2D tex;
    tex.init_dds_from_memory(res.buffer, res.size);
    m.texture(0) = tex.move();

    Model mod{new Mesh3D[1]{std::move(m)}, 1};
    mod.setLocation(
        {parent_size + (calc_distance > 0.f ? calc_size + calc_distance : 0.f),
         0.f, 0.f});
    mod.setModelData(
        new planet_data{orbital_period, rotation_period, axis_tilt});
    mod.setModelPreProcFunction(__preproc_motion);
    mod.setModelPostProcFunction(__postproc_motion);

    for (int i = 0; i < children_count; ++i) {
        mod.addChild(
            children[i]->generate(type == SUN ? calc_size * 2 : calc_size));
    }

    return mod;
}

struct base_phx_t {
    ObjectType type;
    float size;
    float distance;
    uint32_t children;
};

struct calc_phx_t {
	float distance;
    float size;
};

uint32_t do_calc_phx(const base_phx_t *input, calc_phx_t *output,
                     float parent_size) {

    const base_phx_t in = input[0];
    uint32_t c_count = in.children;
    uint32_t processed = 1;
    float calc_size;
    float calc_distance;

    switch (in.type) {
    case SUN:
        calc_size = size_scaling(15.f, in.size);
        calc_distance =
            in.distance > 0.f ? distance_scaling(100000.f, in.distance) : 0.f;
        break;
    case PLANET:
        calc_size = size_scaling(40.f, in.size);
        calc_distance = distance_scaling(200.f, in.distance);
        break;
    case MOON:
        calc_size = size_scaling(255.f, in.size);
        calc_distance = distance_scaling(2000.f, in.distance);
        break;
    default:
        break;
    }

    calc_phx_t &out = output[0];
	out.distance =
		parent_size + (calc_distance > 0.f ? calc_size + calc_distance : 0.f);
    out.size = calc_size / 2.f;

    for (; c_count != 0; --c_count) {
        processed +=
            do_calc_phx(&input[processed], &output[processed], calc_size);
    }
    return processed;
}

void init_home_solar_system(SolarSystem &system, uint32_t model_id) {

    calc_phx_t _calc_phx[21];
    {

        base_phx_t _base_phx[21]{{SUN, 696000.f, 0.f, 8},
                                 {PLANET, 2440.f, 57910000.f, 0},
                                 {PLANET, 6052.f, 108200000.f, 0},
                                 {PLANET, 6371.f, 149597870.691f, 1},
                                 {MOON, 1737.1f, 384400.f, 0},
                                 {PLANET, 3390.f, 227900000.f, 0},
                                 {PLANET, 69911.f, 778500000.f, 4},
                                 {MOON, 1560.8f, 670900.f, 0},
                                 {MOON, 1821.6f, 421700.f, 0},
                                 {MOON, 2410.3f, 1882700.f, 0},
                                 {MOON, 2634.1f, 1070400.f, 0},
                                 {PLANET, 58232.f, 1433000000.f, 7},
                                 {MOON, 2576.f, 1221870.f, 0},
                                 {MOON, 252.f, 238020.f, 0},
                                 {MOON, 763.8f, 527108.f, 0},
                                 {MOON, 561.4f, 377400.f, 0},
                                 {MOON, 531.1f, 294619.f, 0},
                                 {MOON, 734.5f, 3560820.f, 0},
                                 {MOON, 198.2f, 185539.f, 0},
                                 {PLANET, 25362.f, 2870671400.f, 0},
                                 {PLANET, 24622.f, 4503000000.f, 0}};

        uint32_t processed = do_calc_phx(_base_phx, _calc_phx, 0.f);
        ASSERT(processed == 21);
    }

    system.init(21, model_id);

	const uint32_t sun_parts = 64;
	const uint32_t planet_parts = 32;
	const uint32_t moon_parts = 16;

	uint32_t index = 0;
	BitsMemory mem(1024 * 1024 * 2);
	{
		/// SUN
		const uint32_t texture_size = get_res_sun_dds_size();
		const uint32_t vertex_size = sphare::vertex_size(sun_parts);
		const uint32_t index_size = sphare::index_size(sun_parts);

		byte *_buffer =
			(byte *)mem.malloc_aligend_64(texture_size * sizeof(byte));
		get_res_sun_dds(_buffer);

		uv_vertex *vertexes =
			(uv_vertex *)mem.malloc_aligend_64(vertex_size * sizeof(uv_vertex));
		uint32_t *indexes =
			(uint32_t *)mem.malloc_aligend_64(index_size * sizeof(uint32_t));

        sphare::create(vertexes, indexes, _calc_phx[index].size, sun_parts);

		system.set(index, model_phx_data_t{_calc_phx[index].distance,
										   25.6f * day, 0.f, 0.f, 8},
				   model_bin_data_t{_buffer, vertexes, indexes, texture_size,
									vertex_size, index_size});
        mem.clear();
		++index;
    }

    {
		/// MERCURY
		const uint32_t texture_size = get_res_mercury_dds_size();
		const uint32_t vertex_size = sphare::vertex_size(planet_parts);
		const uint32_t index_size = sphare::index_size(planet_parts);

		byte *_buffer =
			(byte *)mem.malloc_aligend_64(texture_size * sizeof(byte));
		get_res_mercury_dds(_buffer);

		uv_vertex *vertexes =
			(uv_vertex *)mem.malloc_aligend_64(vertex_size * sizeof(uv_vertex));
		uint32_t *indexes =
			(uint32_t *)mem.malloc_aligend_64(index_size * sizeof(uint32_t));

		sphare::create(vertexes, indexes, _calc_phx[index].size, planet_parts);

		system.set(index,
				   model_phx_data_t{_calc_phx[index].distance, 58.6f * day,
									88.f * day, 0.1 * degree, 0},
				   model_bin_data_t{_buffer, vertexes, indexes, texture_size,
									vertex_size, index_size});
        mem.clear();
		++index;
    }

    {
		/// VENUS
		const uint32_t texture_size = get_res_venus_dds_size();
		const uint32_t vertex_size = sphare::vertex_size(planet_parts);
		const uint32_t index_size = sphare::index_size(planet_parts);

		byte *_buffer =
			(byte *)mem.malloc_aligend_64(texture_size * sizeof(byte));
		get_res_venus_dds(_buffer);

		uv_vertex *vertexes =
			(uv_vertex *)mem.malloc_aligend_64(vertex_size * sizeof(uv_vertex));
		uint32_t *indexes =
			(uint32_t *)mem.malloc_aligend_64(index_size * sizeof(uint32_t));

		sphare::create(vertexes, indexes, _calc_phx[index].size, planet_parts);

		system.set(index,
				   model_phx_data_t{_calc_phx[index].distance, 116.75f * day,
									224.701f * day, 177.f * degree, 0},
				   model_bin_data_t{_buffer, vertexes, indexes, texture_size,
									vertex_size, index_size});
        mem.clear();
		++index;
    }

    {
		/// EARTH
		const uint32_t texture_size = get_res_earth_dds_size();
		const uint32_t vertex_size = sphare::vertex_size(planet_parts);
		const uint32_t index_size = sphare::index_size(planet_parts);

		byte *_buffer =
			(byte *)mem.malloc_aligend_64(texture_size * sizeof(byte));
		get_res_earth_dds(_buffer);

		uv_vertex *vertexes =
			(uv_vertex *)mem.malloc_aligend_64(vertex_size * sizeof(uv_vertex));
		uint32_t *indexes =
			(uint32_t *)mem.malloc_aligend_64(index_size * sizeof(uint32_t));

		sphare::create(vertexes, indexes, _calc_phx[index].size, planet_parts);

		system.set(index, model_phx_data_t{_calc_phx[index].distance, day, year,
										   23.f * degree, 1},
				   model_bin_data_t{_buffer, vertexes, indexes, texture_size,
									vertex_size, index_size});
        mem.clear();
		++index;
    }

    {
		/// MOON
		const uint32_t texture_size = get_res_moon_dds_size();
		const uint32_t vertex_size = sphare::vertex_size(moon_parts);
		const uint32_t index_size = sphare::index_size(moon_parts);

		byte *_buffer =
			(byte *)mem.malloc_aligend_64(texture_size * sizeof(byte));
		get_res_moon_dds(_buffer);

		uv_vertex *vertexes =
			(uv_vertex *)mem.malloc_aligend_64(vertex_size * sizeof(uv_vertex));
		uint32_t *indexes =
			(uint32_t *)mem.malloc_aligend_64(index_size * sizeof(uint32_t));

		sphare::create(vertexes, indexes, _calc_phx[index].size, moon_parts);

		system.set(index, model_phx_data_t{_calc_phx[index].distance, 0.f,
										   29.530589 * day, 6.687f * degree, 0},
				   model_bin_data_t{_buffer, vertexes, indexes, texture_size,
									vertex_size, index_size});
        mem.clear();
		++index;
    }

    {
		/// MARS
		const uint32_t texture_size = get_res_mars_dds_size();
		const uint32_t vertex_size = sphare::vertex_size(planet_parts);
		const uint32_t index_size = sphare::index_size(planet_parts);

		byte *_buffer =
			(byte *)mem.malloc_aligend_64(texture_size * sizeof(byte));
		get_res_mars_dds(_buffer);

		uv_vertex *vertexes =
			(uv_vertex *)mem.malloc_aligend_64(vertex_size * sizeof(uv_vertex));
		uint32_t *indexes =
			(uint32_t *)mem.malloc_aligend_64(index_size * sizeof(uint32_t));

		sphare::create(vertexes, indexes, _calc_phx[index].size, planet_parts);

		system.set(index, model_phx_data_t{_calc_phx[index].distance,
										   1.02876421707f * day, 686.971f * day,
										   25.f * degree, 0},
				   model_bin_data_t{_buffer, vertexes, indexes, texture_size,
									vertex_size, index_size});
        mem.clear();
		++index;
    }

    {
		/// JUPITER
		const uint32_t texture_size = get_res_jupiter_dds_size();
		const uint32_t vertex_size = sphare::vertex_size(planet_parts);
		const uint32_t index_size = sphare::index_size(planet_parts);

		byte *_buffer =
			(byte *)mem.malloc_aligend_64(texture_size * sizeof(byte));
		get_res_jupiter_dds(_buffer);

		uv_vertex *vertexes =
			(uv_vertex *)mem.malloc_aligend_64(vertex_size * sizeof(uv_vertex));
		uint32_t *indexes =
			(uint32_t *)mem.malloc_aligend_64(index_size * sizeof(uint32_t));

		sphare::create(vertexes, indexes, _calc_phx[index].size, planet_parts);

		system.set(index,
				   model_phx_data_t{_calc_phx[index].distance, 9.97f * hour,
									11.86f * year, 3.f * degree, 4},
				   model_bin_data_t{_buffer, vertexes, indexes, texture_size,
									vertex_size, index_size});
        mem.clear();
		++index;
    }

    {
		/// EUROPA
		const uint32_t texture_size = get_res_europa_dds_size();
		const uint32_t vertex_size = sphare::vertex_size(moon_parts);
		const uint32_t index_size = sphare::index_size(moon_parts);

		byte *_buffer =
			(byte *)mem.malloc_aligend_64(texture_size * sizeof(byte));
		get_res_europa_dds(_buffer);

		uv_vertex *vertexes =
			(uv_vertex *)mem.malloc_aligend_64(vertex_size * sizeof(uv_vertex));
		uint32_t *indexes =
			(uint32_t *)mem.malloc_aligend_64(index_size * sizeof(uint32_t));

		sphare::create(vertexes, indexes, _calc_phx[index].size, moon_parts);

		system.set(index, model_phx_data_t{_calc_phx[index].distance, 0.f,
										   3.551181f * day, 0.1f * degree, 0},
				   model_bin_data_t{_buffer, vertexes, indexes, texture_size,
									vertex_size, index_size});
        mem.clear();
		++index;
    }

    {
		/// IO
		const uint32_t texture_size = get_res_io_dds_size();
		const uint32_t vertex_size = sphare::vertex_size(moon_parts);
		const uint32_t index_size = sphare::index_size(moon_parts);

		byte *_buffer =
			(byte *)mem.malloc_aligend_64(texture_size * sizeof(byte));
		get_res_io_dds(_buffer);

		uv_vertex *vertexes =
			(uv_vertex *)mem.malloc_aligend_64(vertex_size * sizeof(uv_vertex));
		uint32_t *indexes =
			(uint32_t *)mem.malloc_aligend_64(index_size * sizeof(uint32_t));

		sphare::create(vertexes, indexes, _calc_phx[index].size, moon_parts);

		system.set(index,
				   model_phx_data_t{_calc_phx[index].distance, 0.f,
									1.769137786f * day, 0.05f * degree, 0},
				   model_bin_data_t{_buffer, vertexes, indexes, texture_size,
									vertex_size, index_size});
        mem.clear();
		++index;
    }

    {
		/// GANYMADE
		const uint32_t texture_size = get_res_ganymede_dds_size();
		const uint32_t vertex_size = sphare::vertex_size(moon_parts);
		const uint32_t index_size = sphare::index_size(moon_parts);

		byte *_buffer =
			(byte *)mem.malloc_aligend_64(texture_size * sizeof(byte));
		get_res_ganymede_dds(_buffer);

		uv_vertex *vertexes =
			(uv_vertex *)mem.malloc_aligend_64(vertex_size * sizeof(uv_vertex));
		uint32_t *indexes =
			(uint32_t *)mem.malloc_aligend_64(index_size * sizeof(uint32_t));

		sphare::create(vertexes, indexes, _calc_phx[index].size, moon_parts);

		system.set(index,
				   model_phx_data_t{_calc_phx[index].distance, 0.f,
									7.15455296f * day, 2.214f * degree, 0},
				   model_bin_data_t{_buffer, vertexes, indexes, texture_size,
									vertex_size, index_size});
        mem.clear();
		++index;
    }

    {
		/// CALLISTO
		const uint32_t texture_size = get_res_callisto_dds_size();
		const uint32_t vertex_size = sphare::vertex_size(moon_parts);
		const uint32_t index_size = sphare::index_size(moon_parts);

		byte *_buffer =
			(byte *)mem.malloc_aligend_64(texture_size * sizeof(byte));
		get_res_callisto_dds(_buffer);

		uv_vertex *vertexes =
			(uv_vertex *)mem.malloc_aligend_64(vertex_size * sizeof(uv_vertex));
		uint32_t *indexes =
			(uint32_t *)mem.malloc_aligend_64(index_size * sizeof(uint32_t));

		sphare::create(vertexes, indexes, _calc_phx[index].size, moon_parts);

		system.set(index,
				   model_phx_data_t{_calc_phx[index].distance, 0.f,
									16.6890184f * day, 2.017f * degree, 0},
				   model_bin_data_t{_buffer, vertexes, indexes, texture_size,
									vertex_size, index_size});
        mem.clear();
		++index;
    }

    {
		/// SATURN
		const uint32_t texture_size = get_res_saturn_dds_size();
		const uint32_t vertex_size = sphare::vertex_size(planet_parts);
		const uint32_t index_size = sphare::index_size(planet_parts);

		byte *_buffer =
			(byte *)mem.malloc_aligend_64(texture_size * sizeof(byte));
		get_res_saturn_dds(_buffer);

		uv_vertex *vertexes =
			(uv_vertex *)mem.malloc_aligend_64(vertex_size * sizeof(uv_vertex));
		uint32_t *indexes =
			(uint32_t *)mem.malloc_aligend_64(index_size * sizeof(uint32_t));

		sphare::create(vertexes, indexes, _calc_phx[index].size, planet_parts);

		system.set(index,
				   model_phx_data_t{_calc_phx[index].distance, 10.65f * hour,
									29.f * year, 26.7f * degree, 7},
				   model_bin_data_t{_buffer, vertexes, indexes, texture_size,
									vertex_size, index_size});
        mem.clear();
		++index;
    }

    {
		/// TITAN
		const uint32_t texture_size = get_res_titan_dds_size();
		const uint32_t vertex_size = sphare::vertex_size(moon_parts);
		const uint32_t index_size = sphare::index_size(moon_parts);

		byte *_buffer =
			(byte *)mem.malloc_aligend_64(texture_size * sizeof(byte));
		get_res_titan_dds(_buffer);

		uv_vertex *vertexes =
			(uv_vertex *)mem.malloc_aligend_64(vertex_size * sizeof(uv_vertex));
		uint32_t *indexes =
			(uint32_t *)mem.malloc_aligend_64(index_size * sizeof(uint32_t));

		sphare::create(vertexes, indexes, _calc_phx[index].size, moon_parts);

		system.set(index, model_phx_data_t{_calc_phx[index].distance, 0.f,
										   15.945f * day, 0.f, 0},
				   model_bin_data_t{_buffer, vertexes, indexes, texture_size,
									vertex_size, index_size});
        mem.clear();
		++index;
    }

    {
		/// ENCELADUS
		const uint32_t texture_size = get_res_enceladus_dds_size();
		const uint32_t vertex_size = sphare::vertex_size(moon_parts);
		const uint32_t index_size = sphare::index_size(moon_parts);

		byte *_buffer =
			(byte *)mem.malloc_aligend_64(texture_size * sizeof(byte));
		get_res_enceladus_dds(_buffer);

		uv_vertex *vertexes =
			(uv_vertex *)mem.malloc_aligend_64(vertex_size * sizeof(uv_vertex));
		uint32_t *indexes =
			(uint32_t *)mem.malloc_aligend_64(index_size * sizeof(uint32_t));

		sphare::create(vertexes, indexes, _calc_phx[index].size, moon_parts);

		system.set(index, model_phx_data_t{_calc_phx[index].distance, 0.f,
										   1.370218f * day, 0.017f * degree, 0},
				   model_bin_data_t{_buffer, vertexes, indexes, texture_size,
									vertex_size, index_size});
        mem.clear();
		++index;
    }

    {
		/// RHEA
		const uint32_t texture_size = get_res_rhea_dds_size();
		const uint32_t vertex_size = sphare::vertex_size(moon_parts);
		const uint32_t index_size = sphare::index_size(moon_parts);

		byte *_buffer =
			(byte *)mem.malloc_aligend_64(texture_size * sizeof(byte));
		get_res_rhea_dds(_buffer);

		uv_vertex *vertexes =
			(uv_vertex *)mem.malloc_aligend_64(vertex_size * sizeof(uv_vertex));
		uint32_t *indexes =
			(uint32_t *)mem.malloc_aligend_64(index_size * sizeof(uint32_t));

		sphare::create(vertexes, indexes, _calc_phx[index].size, moon_parts);

		system.set(index, model_phx_data_t{_calc_phx[index].distance, 0.f,
										   4.518212f * day, 0.f, 0},
				   model_bin_data_t{_buffer, vertexes, indexes, texture_size,
									vertex_size, index_size});
        mem.clear();
		++index;
    }

    {
		/// DIONE
		const uint32_t texture_size = get_res_dione_dds_size();
		const uint32_t vertex_size = sphare::vertex_size(moon_parts);
		const uint32_t index_size = sphare::index_size(moon_parts);

		byte *_buffer =
			(byte *)mem.malloc_aligend_64(texture_size * sizeof(byte));
		get_res_dione_dds(_buffer);

		uv_vertex *vertexes =
			(uv_vertex *)mem.malloc_aligend_64(vertex_size * sizeof(uv_vertex));
		uint32_t *indexes =
			(uint32_t *)mem.malloc_aligend_64(index_size * sizeof(uint32_t));

		sphare::create(vertexes, indexes, _calc_phx[index].size, moon_parts);

		system.set(index, model_phx_data_t{_calc_phx[index].distance, 0.f,
										   2.736915f * day, 0.f, 0},
				   model_bin_data_t{_buffer, vertexes, indexes, texture_size,
									vertex_size, index_size});
        mem.clear();
		++index;
    }

    {
		/// TETHYS
		const uint32_t texture_size = get_res_tethys_dds_size();
		const uint32_t vertex_size = sphare::vertex_size(moon_parts);
		const uint32_t index_size = sphare::index_size(moon_parts);

		byte *_buffer =
			(byte *)mem.malloc_aligend_64(texture_size * sizeof(byte));
		get_res_tethys_dds(_buffer);

		uv_vertex *vertexes =
			(uv_vertex *)mem.malloc_aligend_64(vertex_size * sizeof(uv_vertex));
		uint32_t *indexes =
			(uint32_t *)mem.malloc_aligend_64(index_size * sizeof(uint32_t));

		sphare::create(vertexes, indexes, _calc_phx[index].size, moon_parts);

		system.set(index, model_phx_data_t{_calc_phx[index].distance, 0.f,
										   1.887802f * day, 0.f, 0},
				   model_bin_data_t{_buffer, vertexes, indexes, texture_size,
									vertex_size, index_size});
        mem.clear();
		++index;
    }

    {
		/// IAPETUS
		const uint32_t texture_size = get_res_iapetus_dds_size();
		const uint32_t vertex_size = sphare::vertex_size(moon_parts);
		const uint32_t index_size = sphare::index_size(moon_parts);

		byte *_buffer =
			(byte *)mem.malloc_aligend_64(texture_size * sizeof(byte));
		get_res_iapetus_dds(_buffer);

		uv_vertex *vertexes =
			(uv_vertex *)mem.malloc_aligend_64(vertex_size * sizeof(uv_vertex));
		uint32_t *indexes =
			(uint32_t *)mem.malloc_aligend_64(index_size * sizeof(uint32_t));

		sphare::create(vertexes, indexes, _calc_phx[index].size, moon_parts);

		system.set(index, model_phx_data_t{_calc_phx[index].distance, 0.f,
										   79.3215f * day, 0.f, 0},
				   model_bin_data_t{_buffer, vertexes, indexes, texture_size,
									vertex_size, index_size});
        mem.clear();
		++index;
    }

    {
		/// MIMAS
		const uint32_t texture_size = get_res_mimas_dds_size();
		const uint32_t vertex_size = sphare::vertex_size(moon_parts);
		const uint32_t index_size = sphare::index_size(moon_parts);

		byte *_buffer =
			(byte *)mem.malloc_aligend_64(texture_size * sizeof(byte));
		get_res_mimas_dds(_buffer);

		uv_vertex *vertexes =
			(uv_vertex *)mem.malloc_aligend_64(vertex_size * sizeof(uv_vertex));
		uint32_t *indexes =
			(uint32_t *)mem.malloc_aligend_64(index_size * sizeof(uint32_t));

		sphare::create(vertexes, indexes, _calc_phx[index].size, moon_parts);

		system.set(index, model_phx_data_t{_calc_phx[index].distance, 0.f,
										   0.942f * day, 0.f, 0},
				   model_bin_data_t{_buffer, vertexes, indexes, texture_size,
									vertex_size, index_size});
        mem.clear();
		++index;
    }

    {
		/// URANUS
		const uint32_t texture_size = get_res_uranus_dds_size();
		const uint32_t vertex_size = sphare::vertex_size(planet_parts);
		const uint32_t index_size = sphare::index_size(planet_parts);

		byte *_buffer =
			(byte *)mem.malloc_aligend_64(texture_size * sizeof(byte));
		get_res_uranus_dds(_buffer);

		uv_vertex *vertexes =
			(uv_vertex *)mem.malloc_aligend_64(vertex_size * sizeof(uv_vertex));
		uint32_t *indexes =
			(uint32_t *)mem.malloc_aligend_64(index_size * sizeof(uint32_t));

		sphare::create(vertexes, indexes, _calc_phx[index].size, planet_parts);

		system.set(index,
				   model_phx_data_t{_calc_phx[index].distance,
									17.233333333f * hour, 84.016846f * year,
									97.77f * degree, 0},
				   model_bin_data_t{_buffer, vertexes, indexes, texture_size,
									vertex_size, index_size});
        mem.clear();
		++index;
    }
	{
		/// NEPTUNE
		const uint32_t texture_size = get_res_neptune_dds_size();
		const uint32_t vertex_size = sphare::vertex_size(planet_parts);
		const uint32_t index_size = sphare::index_size(planet_parts);

		byte *_buffer =
			(byte *)mem.malloc_aligend_64(texture_size * sizeof(byte));
		get_res_neptune_dds(_buffer);

		uv_vertex *vertexes =
			(uv_vertex *)mem.malloc_aligend_64(vertex_size * sizeof(uv_vertex));
		uint32_t *indexes =
			(uint32_t *)mem.malloc_aligend_64(index_size * sizeof(uint32_t));

		sphare::create(vertexes, indexes, _calc_phx[index].size, planet_parts);

		system.set(index, model_phx_data_t{_calc_phx[index].distance,
										   16.266666667f * hour, 165 * year,
										   28.32f * degree, 0},
				   model_bin_data_t{_buffer, vertexes, indexes, texture_size,
									vertex_size, index_size});
        mem.clear();
		++index;
    }
}

glProgram::glProgram() {

    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(1);
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, nullptr,
                                nullptr);
    glfwMakeContextCurrent(m_window);

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        exit(1);
    } else {
        if (!GLEW_VERSION_3_3) {
            fprintf(stderr, "OpenGL 3.3 is not supported\n");
            exit(1);
        }
    }

    glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetCursorPos(m_window, halfWindowWidth, halfWindowHeight);
    m_lamp = {glm::vec3(0.f, 0.f, 5.f), 500.f};

    Viewport::update({0, 0, windowWidth, windowHeight});

    glClearColor(0.05f, 0.075f, 0.1f, 1.0f);

    glClearDepth(1.0f);
    glDepthFunc(GL_LESS);

    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    init_resources();

    {
        Shader _solar_shaders[2]{{VERTEX, get_res_object_vert_glsl()},
                                 {FRAGMENT, get_res_object_frag_glsl()}};
        program.program_id = ProgramCompiler::compileProgram(_solar_shaders, 2);

        const int _uni_size = 4;
        uint32_t _solar_uni[_uni_size];
        ProgramCompiler::resolveUniforms(program.program_id, _solar_uni,
                                         _uni_size);
        program.projection_id =
            Uniforms::getUniformId(_solar_uni, _uni_size, "projectionMatrix"_h);
        program.view_id =
            Uniforms::getUniformId(_solar_uni, _uni_size, "viewMatrix"_h);
        program.model_id =
            Uniforms::getUniformId(_solar_uni, _uni_size, "modelMatrix"_h);
    }

	init_home_solar_system(system, program.model_id);

    //    m_objectProgram.createShader({VERTEX, get_res_object_vert_glsl()});
    //    m_objectProgram.createShader({FRAGMENT, get_res_object_frag_glsl()});
    //    m_objectProgram.createProgram();
    //    m_objectProgram.linkProgram();
    //    m_objectProgram.resolveUniforms();

    //    m_objectSelectionProgram.createShader(
    //        {VERTEX, get_res_object_select_vert_glsl()});
    //    m_objectSelectionProgram.createShader(
    //        {FRAGMENT, get_res_object_select_frag_glsl()});
    //    m_objectSelectionProgram.createProgram();
    //    m_objectSelectionProgram.linkProgram();
    //    m_objectSelectionProgram.resolveUniforms();

    //    m_axis.init();

	//    float mercury_distance = 57910000.f;
	//    float venus_distance = 108200000.f;
	//    float earth_distance = 149597870.691f;
	//    float moon_distance = 384400.f;
	//    float mars_distance = 227900000.f;
	//    float jupiter_distance = 778500000.f;
	//    float europa_distance = 670900.f;
	//    float io_distance = 421700.f;
	//    float callisto_distance = 1882700.f;
	//    float ganymede_distance = 1070400.f;
	//    float saturn_distance = 1433000000.f;

	//    float sun_size = 696000.f;
	//    float mercury_size = 2440.f;
	//    float venus_size = 6052.f;
	//    float earth_size = 6371.f;
	//    float moon_size = 1737.1f;
	//    float mars_size = 3390.f;
	//    float jupiter_size = 69911.f;
	//    float europa_size = 1560.8f;
	//    float io_size = 1821.6f;
	//    float callisto_size = 2410.3f;
	//    float ganymede_size = 2634.1f;
	//    float saturn_size = 58232.f;

	//    BitsMemory mem(1024 * 1024 * 20);
	//    //    set_malloc_function(BitsMemory::malloc_aligend_64);

	//    Planet sun;
	//    sun.setup("sun", sun_size, 0.f, 0.f, 25.6f * day,
	//              {0.f, 0.f, 0.f /*7.25f * degree*/}, SUN,
	//              get_res_sun_dds(mem.malloc_aligend_32(get_res_sun_dds_size())));

	//    Planet mercury;
	//    mercury.setup(
	//        "mercury", mercury_size, mercury_distance, day * 88.f, day *
	//        58.6f,
	//        {0.f, 0.f, 0.1 * degree}, PLANET,
	//        get_res_mercury_dds(mem.malloc_aligend_32(get_res_mercury_dds_size())));

	//    Planet venus;
	//    venus.setup(
	//        "venus", venus_size, venus_distance, day * 224.701f, day *
	//        116.75f,
	//        {0.f, 0.f, 177.f * degree}, PLANET,
	//        get_res_venus_dds(mem.malloc_aligend_32(get_res_venus_dds_size())));

	//    Planet earth;
	//    earth.setup(
	//        "earth", earth_size, earth_distance, year, day,
	//        {0.f, 0.f, 23.f * degree}, PLANET,
	//        get_res_earth_dds(mem.malloc_aligend_32(get_res_earth_dds_size())));

	//    Planet moon;
	//    moon.setup(
	//        "moon", moon_size, moon_distance, day * 29.530589f, 0.f,
	//        {0.f, 0.f, 6.687f * degree}, MOON,
	//        get_res_moon_dds(mem.malloc_aligend_32(get_res_moon_dds_size())));

	//    earth.setChildrenCount(1);
	//    earth.setChild(0, &moon);

	//    Planet mars;
	//    mars.setup(
	//        "mars", mars_size, mars_distance, 686.971f * day, 1.02876421707f *
	//        day,
	//        {0.f, 0.f, 25.f * degree}, PLANET,
	//        get_res_mars_dds(mem.malloc_aligend_32(get_res_mars_dds_size())));

	//    Planet jupiter;
	//    jupiter.setup(
	//        "jupiter", jupiter_size, jupiter_distance, 11.86f * year, 9.97f *
	//        hour,
	//        {0.f, 0.f, 3.f * degree}, PLANET,
	//        get_res_jupiter_dds(mem.malloc_aligend_32(get_res_jupiter_dds_size())));

	//    Planet europa;
	//    europa.setup(
	//        "europa", europa_size, europa_distance, 3.551181f * day, 0.f,
	//        {0.f, 0.f, 0.1f * degree}, MOON,
	//        get_res_europa_dds(mem.malloc_aligend_32(get_res_europa_dds_size())));

	//    Planet io;
	//    io.setup("io", io_size, io_distance, 1.769137786f * day, 0.f,
	//             {0.f, 0.f, 0.05f * degree}, MOON,
	//             get_res_io_dds(mem.malloc_aligend_32(get_res_io_dds_size())));

	//    Planet ganymede;
	//    ganymede.setup("ganymede", ganymede_size, ganymede_distance,
	//                   7.15455296f * day, 0.f, {0.f, 0.f, 2.214f * degree},
	//                   MOON,
	//                   get_res_ganymede_dds(
	//                       mem.malloc_aligend_32(get_res_ganymede_dds_size())));

	//    Planet callisto;
	//    callisto.setup("callisto", callisto_size, callisto_distance,
	//                   16.6890184f * day, 0.f, {0.f, 0.f, 2.017f * degree},
	//                   MOON,
	//                   get_res_callisto_dds(
	//                       mem.malloc_aligend_32(get_res_callisto_dds_size())));

	//    jupiter.setChildrenCount(4);
	//    jupiter.setChild(0, &europa);
	//    jupiter.setChild(1, &io);
	//    jupiter.setChild(2, &ganymede);
	//    jupiter.setChild(3, &callisto);

	//    Planet saturn;
	//    saturn.setup(
	//        "saturn", saturn_size, saturn_distance, 29.f * year, 10.65f *
	//        hour,
	//        {0.f, 0.f, 26.7f * degree}, PLANET,
	//        get_res_saturn_dds(mem.malloc_aligend_32(get_res_saturn_dds_size())));

	//    Planet titan;
	//    titan.setup(
	//        "titan", 2576.f, 1221870.f, 15.945f * day, 0.f,
	//        {0.f, 0.f, 0.f * degree}, MOON,
	//        get_res_titan_dds(mem.malloc_aligend_32(get_res_titan_dds_size())));

	//    Planet enceladus;
	//    enceladus.setup("enceladus", 252.f, 238020.f, 1.370218f * day, 0.f,
	//                    {0.f, 0.f, 0.017f * degree}, MOON,
	//                    get_res_enceladus_dds(
	//                        mem.malloc_aligend_32(get_res_enceladus_dds_size())));

	//    Planet rhea;
	//    rhea.setup(
	//        "rhea", 763.8f, 527108.f, 4.518212f * day, 0.f,
	//        {0.f, 0.f, 0.f * degree}, MOON,
	//        get_res_rhea_dds(mem.malloc_aligend_32(get_res_rhea_dds_size())));

	//    Planet dione;
	//    dione.setup(
	//        "dione", 561.4f, 377400.f, 2.736915f * day, 0.f,
	//        {0.f, 0.f, 0.f * degree}, MOON,
	//        get_res_dione_dds(mem.malloc_aligend_32(get_res_dione_dds_size())));

	//    Planet tethys;
	//    tethys.setup(
	//        "tethys", 531.1f, 294619.f, 1.887802f * day, 0.f,
	//        {0.f, 0.f, 0.f * degree}, MOON,
	//        get_res_tethys_dds(mem.malloc_aligend_32(get_res_tethys_dds_size())));

	//    Planet iapetus;
	//    iapetus.setup(
	//        "iapetus", 734.5f, 3560820.f, 79.3215f * day, 0.f,
	//        {0.f, 0.f, 0.f * degree}, MOON,
	//        get_res_iapetus_dds(mem.malloc_aligend_32(get_res_iapetus_dds_size())));

	//    Planet mimas;
	//    mimas.setup(
	//        "mimas", 198.2f, 185539.f, 0.942f * day, 0.f, {0.f, 0.f, 0.f *
	//        degree},
	//        MOON,
	//        get_res_mimas_dds(mem.malloc_aligend_32(get_res_mimas_dds_size())));

	//    saturn.setChildrenCount(7);
	//    saturn.setChild(0, &titan);
	//    saturn.setChild(1, &enceladus);
	//    saturn.setChild(2, &rhea);
	//    saturn.setChild(3, &dione);
	//    saturn.setChild(4, &tethys);
	//    saturn.setChild(5, &iapetus);
	//    saturn.setChild(6, &mimas);

	//    Planet uranus;
	//    uranus.setup(
	//        "uranus", 25362.f, 2870671400.f, 84.016846f * year,
	//        17.233333333f * hour, {0.f, 0.f, 97.77f * degree}, PLANET,
	//        get_res_uranus_dds(mem.malloc_aligend_32(get_res_uranus_dds_size())));

	//    Planet neptune;
	//    neptune.setup(
	//        "neptune", 24622.f, 4503000000.f, 165 * year, 16.266666667f *
	//        hour,
	//        {0.f, 0.f, 28.32f * degree}, PLANET,
	//        get_res_neptune_dds(mem.malloc_aligend_32(get_res_neptune_dds_size())));

	//    sun.setChildrenCount(8);
	//    sun.setChild(0, &mercury);
	//    sun.setChild(1, &venus);
	//    sun.setChild(2, &earth);
	//    sun.setChild(3, &mars);
	//    sun.setChild(4, &jupiter);
	//    sun.setChild(5, &saturn);
	//    sun.setChild(6, &uranus);
	//    sun.setChild(7, &neptune);

	//    m_models.push_back(sun.generate());

    {
        Shader text_[2]{{VERTEX, get_res_text_vert_glsl()},
                        {FRAGMENT, get_res_text_frag_glsl()}};

        Text::init(text_, 2, "projectionMatrix"_h, "modelMatrix"_h, "colour"_h);
    }

	{

		const uint32_t ttf_size = get_res_freesans_ttf_size();
		byte ttf_buffer[ttf_size];

		get_res_freesans_ttf(ttf_buffer);
		m_text = {ttf_buffer, ttf_size, 32};
		m_text.setColour(colour(150, 160, 170, 255));
	}
    //    glDebugger::init({freesans_buffer.buffer, freesans_buffer.size, 14});

    deinit_resources();

    m_mouse = {m_window};
    m_keyboard = {m_window};

    m_mouse.onClickLeft(std::bind(&glProgram::handleSelection, this,
                                  std::placeholders::_1,
                                  std::placeholders::_2));

    m_mouse.onDragLeft([=](MoveDirection event, float diff) {
        if (event == MOVE_Y) {
            m_camera.move(diff);
        } else if (event == MOVE_X) {
            m_camera.strafe(diff);
        } else if (event == MOVE_Z) {
            m_camera.zoom(diff);
        }
    });

    m_mouse.onDragRight([=](MoveDirection event, float diff) {
        if (event == MOVE_Y) {
            m_camera.rotateVertical(diff);
        } else if (event == MOVE_X) {
            m_camera.rotateHorizontal(diff);
        } else if (event == MOVE_Z) {
            m_camera.zoom(diff);
        }
    });

    m_keyboard.moveX([=](float diff) { m_camera.move(diff); });

    m_keyboard.moveY([=](float diff) { m_camera.strafe(diff); });

    m_keyboard.moveZ([=](float diff) { m_camera.zoom(diff); });

    m_keyboard.rotateHorizontal(
        [=](float diff) { m_camera.rotateHorizontal(diff); });

    m_keyboard.rotateVertical(
        [=](float diff) { m_camera.rotateVertical(diff); });

    glfwSwapInterval(1);
}

void glProgram::exec() {
    Clock::update();
    uint32_t frame = 0;
    do {
        uint64_t start = Clock::now();
        glfwPollEvents();
        m_mouse.update();
        m_keyboard.update();
        render();
        Clock::update();
        if (++frame > 60) {
            float rate = 1000000000.f / (Clock::now() - start);
            if (rate > m_maxFrameRate)
                m_maxFrameRate = rate;
            else if (rate < m_minFrameRate)
                m_minFrameRate = rate;
        }
		if(frame == 1000) return;

    } while (glfwGetKey(m_window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             glfwWindowShouldClose(m_window) == 0);
}

void glProgram::render() {
    PROF("Render loop iteration");
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        {
            PROF("Setup before render");
            m_camera.update();
            glUseProgram(program.program_id);
			Uniforms::setUniform(program.projection_id, m_camera.projection());
			Uniforms::setUniform(program.view_id, m_camera.view());
        }
		{
			PROF("Prepare all objects");
			system.prepare();
		}

        {
            PROF("Rendering all objects");
			system.render();
        }
        {
            PROF("Creating frame rate text");
            char buf[128];
            std::sprintf(buf, "MAX FPS: %.2f\nMIN FPS: %.2f", m_maxFrameRate,
                         m_minFrameRate);
            {
                PROF("Rendering text");
                Text::beginRender();
                m_text.render(buf, 20, 40);
                Text::endRender();
            }
        }
    }

    glfwSwapBuffers(m_window);
}

void glProgram::handleSelection(double x, double y) {
    //    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //    m_objectSelectionProgram.use();
    //    m_camera.update(m_objectSelectionProgram);

    //    for (auto &m : m_models) {
    //        m.render(m_objectSelectionProgram);
    //    }

    //    unsigned char res[4]{0};

    //    double viewportheight = m_objectSelectionProgram.getViewport().h;
    //    double screenX = x;
    //    double screenY = viewportheight - y;
    //    glReadPixels(screenX, screenY, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, res);

    //    auto it = std::find(std::begin(m_models), std::end(m_models),
    //                        ColourID(res[0], res[1], res[2]).value());
    //    if (it != std::end(m_models)) {
    //        it->select(!it->selected());
    //    }
}
