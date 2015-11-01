#include "glprogram.h"
#include "Memory/bitsmemory.h"

#include "Common/str_hash.h"

#include "Util/clock.h"
#include "Util/mouse.h"
#include "Util/shapes.h"
#include "Util/gldebugger.h"
#include "Util/profiler.h"
#include "Util/viewport.h"
#include "Util/texture_2d.h"
#include "Util/mesh_3d.h"
#include "Util/glvalidator.h"
#include "Util/helperfunctions.h"
#include "Util/buffer_store.h"

#include "resources/resourcemanager.h"

#include <stdio.h>

constexpr float year{30 * 60 * 1000.f};
constexpr float size_div{6371.f};
constexpr float distance_div{14959.78707f};
constexpr float rat{distance_div / size_div};
constexpr float days_in_year{365.256f};
constexpr float day{year / days_in_year};
constexpr float hour{day / 23.9344722f};
constexpr float degree{0.0174532925f};

float size_scaling(float amp, float x) {
    return std::sqrt((x / (amp * 2)) + std::sqrt(amp / 25));
}

float distance_scaling(float amp, float x) {
    return std::sqrt((x / (amp * 2)) + std::sqrt(amp / 25));
}

enum ObjectType { SUN, PLANET, MOON, SHALLOW_MOON };

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
        calc_size = size_scaling(20.f, in.size);
        calc_distance = distance_scaling(100.f, in.distance);
        break;
    case MOON:
        calc_size = size_scaling(55.f, in.size);
        calc_distance = distance_scaling(200.f, in.distance);
        break;
    default:
        calc_size = 0;
        calc_distance = 0;
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

        uint32_t processed_count = do_calc_phx(_base_phx, _calc_phx, 0.f);
        ASSERT(processed_count == 21);
    }

    system.init(21, model_id);

    const uint32_t sun_parts = 64;
    const uint32_t planet_parts = 64;
    const uint32_t moon_parts = 32;

    uint32_t index = 0;
    BitsMemory mem(1024 * 1024 * 50);
    {
        /// SUN
        const uint32_t texture_size = get_res_sun_jpg_size();
        const uint32_t vertex_size = sphare::vertex_size(sun_parts);
        const uint32_t index_size = sphare::index_size(sun_parts);

        byte *_buffer =
            (byte *)mem.malloc_aligend_64(texture_size * sizeof(byte));
        get_res_sun_jpg(_buffer);

        obj_3d_vertex *vertexes = (obj_3d_vertex *)mem.malloc_aligend_64(
            vertex_size * sizeof(obj_3d_vertex));
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
        const uint32_t texture_size = get_res_mercury_jpg_size();
        const uint32_t vertex_size = sphare::vertex_size(planet_parts);
        const uint32_t index_size = sphare::index_size(planet_parts);

        byte *_buffer =
            (byte *)mem.malloc_aligend_64(texture_size * sizeof(byte));
        get_res_mercury_jpg(_buffer);

        obj_3d_vertex *vertexes = (obj_3d_vertex *)mem.malloc_aligend_64(
            vertex_size * sizeof(obj_3d_vertex));
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
        const uint32_t texture_size = get_res_venus_jpg_size();
        const uint32_t vertex_size = sphare::vertex_size(planet_parts);
        const uint32_t index_size = sphare::index_size(planet_parts);

        byte *_buffer =
            (byte *)mem.malloc_aligend_64(texture_size * sizeof(byte));
        get_res_venus_jpg(_buffer);

        obj_3d_vertex *vertexes = (obj_3d_vertex *)mem.malloc_aligend_64(
            vertex_size * sizeof(obj_3d_vertex));
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
        const uint32_t texture_size = get_res_earth_jpg_size();
        const uint32_t vertex_size = sphare::vertex_size(planet_parts);
        const uint32_t index_size = sphare::index_size(planet_parts);

        byte *_buffer =
            (byte *)mem.malloc_aligend_64(texture_size * sizeof(byte));
        get_res_earth_jpg(_buffer);

        obj_3d_vertex *vertexes = (obj_3d_vertex *)mem.malloc_aligend_64(
            vertex_size * sizeof(obj_3d_vertex));
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
        const uint32_t texture_size = get_res_moon_jpg_size();
        const uint32_t vertex_size = sphare::vertex_size(moon_parts);
        const uint32_t index_size = sphare::index_size(moon_parts);

        byte *_buffer =
            (byte *)mem.malloc_aligend_64(texture_size * sizeof(byte));
        get_res_moon_jpg(_buffer);

        obj_3d_vertex *vertexes = (obj_3d_vertex *)mem.malloc_aligend_64(
            vertex_size * sizeof(obj_3d_vertex));
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
        const uint32_t texture_size = get_res_mars_jpg_size();
        const uint32_t vertex_size = sphare::vertex_size(planet_parts);
        const uint32_t index_size = sphare::index_size(planet_parts);

        byte *_buffer =
            (byte *)mem.malloc_aligend_64(texture_size * sizeof(byte));
        get_res_mars_jpg(_buffer);

        obj_3d_vertex *vertexes = (obj_3d_vertex *)mem.malloc_aligend_64(
            vertex_size * sizeof(obj_3d_vertex));
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
        const uint32_t texture_size = get_res_jupiter_jpg_size();
        const uint32_t vertex_size = sphare::vertex_size(planet_parts);
        const uint32_t index_size = sphare::index_size(planet_parts);

        byte *_buffer =
            (byte *)mem.malloc_aligend_64(texture_size * sizeof(byte));
        get_res_jupiter_jpg(_buffer);

        obj_3d_vertex *vertexes = (obj_3d_vertex *)mem.malloc_aligend_64(
            vertex_size * sizeof(obj_3d_vertex));
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
        const uint32_t texture_size = get_res_europa_jpg_size();
        const uint32_t vertex_size = sphare::vertex_size(moon_parts);
        const uint32_t index_size = sphare::index_size(moon_parts);

        byte *_buffer =
            (byte *)mem.malloc_aligend_64(texture_size * sizeof(byte));
        get_res_europa_jpg(_buffer);

        obj_3d_vertex *vertexes = (obj_3d_vertex *)mem.malloc_aligend_64(
            vertex_size * sizeof(obj_3d_vertex));
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
        const uint32_t texture_size = get_res_io_jpg_size();
        const uint32_t vertex_size = sphare::vertex_size(moon_parts);
        const uint32_t index_size = sphare::index_size(moon_parts);

        byte *_buffer =
            (byte *)mem.malloc_aligend_64(texture_size * sizeof(byte));
        get_res_io_jpg(_buffer);

        obj_3d_vertex *vertexes = (obj_3d_vertex *)mem.malloc_aligend_64(
            vertex_size * sizeof(obj_3d_vertex));
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
        const uint32_t texture_size = get_res_ganymede_jpg_size();
        const uint32_t vertex_size = sphare::vertex_size(moon_parts);
        const uint32_t index_size = sphare::index_size(moon_parts);

        byte *_buffer =
            (byte *)mem.malloc_aligend_64(texture_size * sizeof(byte));
        get_res_ganymede_jpg(_buffer);

        obj_3d_vertex *vertexes = (obj_3d_vertex *)mem.malloc_aligend_64(
            vertex_size * sizeof(obj_3d_vertex));
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
        const uint32_t texture_size = get_res_callisto_jpg_size();
        const uint32_t vertex_size = sphare::vertex_size(moon_parts);
        const uint32_t index_size = sphare::index_size(moon_parts);

        byte *_buffer =
            (byte *)mem.malloc_aligend_64(texture_size * sizeof(byte));
        get_res_callisto_jpg(_buffer);

        obj_3d_vertex *vertexes = (obj_3d_vertex *)mem.malloc_aligend_64(
            vertex_size * sizeof(obj_3d_vertex));
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
        const uint32_t texture_size = get_res_saturn_jpg_size();
        const uint32_t vertex_size = sphare::vertex_size(planet_parts);
        const uint32_t index_size = sphare::index_size(planet_parts);

        byte *_buffer =
            (byte *)mem.malloc_aligend_64(texture_size * sizeof(byte));
        get_res_saturn_jpg(_buffer);

        obj_3d_vertex *vertexes = (obj_3d_vertex *)mem.malloc_aligend_64(
            vertex_size * sizeof(obj_3d_vertex));
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
        const uint32_t texture_size = get_res_titan_jpg_size();
        const uint32_t vertex_size = sphare::vertex_size(moon_parts);
        const uint32_t index_size = sphare::index_size(moon_parts);

        byte *_buffer =
            (byte *)mem.malloc_aligend_64(texture_size * sizeof(byte));
        get_res_titan_jpg(_buffer);

        obj_3d_vertex *vertexes = (obj_3d_vertex *)mem.malloc_aligend_64(
            vertex_size * sizeof(obj_3d_vertex));
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
        const uint32_t texture_size = get_res_enceladus_jpg_size();
        const uint32_t vertex_size = sphare::vertex_size(moon_parts);
        const uint32_t index_size = sphare::index_size(moon_parts);

        byte *_buffer =
            (byte *)mem.malloc_aligend_64(texture_size * sizeof(byte));
        get_res_enceladus_jpg(_buffer);

        obj_3d_vertex *vertexes = (obj_3d_vertex *)mem.malloc_aligend_64(
            vertex_size * sizeof(obj_3d_vertex));
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
        const uint32_t texture_size = get_res_rhea_jpg_size();
        const uint32_t vertex_size = sphare::vertex_size(moon_parts);
        const uint32_t index_size = sphare::index_size(moon_parts);

        byte *_buffer =
            (byte *)mem.malloc_aligend_64(texture_size * sizeof(byte));
        get_res_rhea_jpg(_buffer);

        obj_3d_vertex *vertexes = (obj_3d_vertex *)mem.malloc_aligend_64(
            vertex_size * sizeof(obj_3d_vertex));
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
        const uint32_t texture_size = get_res_dione_jpg_size();
        const uint32_t vertex_size = sphare::vertex_size(moon_parts);
        const uint32_t index_size = sphare::index_size(moon_parts);

        byte *_buffer =
            (byte *)mem.malloc_aligend_64(texture_size * sizeof(byte));
        get_res_dione_jpg(_buffer);

        obj_3d_vertex *vertexes = (obj_3d_vertex *)mem.malloc_aligend_64(
            vertex_size * sizeof(obj_3d_vertex));
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
        const uint32_t texture_size = get_res_tethys_jpg_size();
        const uint32_t vertex_size = sphare::vertex_size(moon_parts);
        const uint32_t index_size = sphare::index_size(moon_parts);

        byte *_buffer =
            (byte *)mem.malloc_aligend_64(texture_size * sizeof(byte));
        get_res_tethys_jpg(_buffer);

        obj_3d_vertex *vertexes = (obj_3d_vertex *)mem.malloc_aligend_64(
            vertex_size * sizeof(obj_3d_vertex));
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
        const uint32_t texture_size = get_res_iapetus_jpg_size();
        const uint32_t vertex_size = sphare::vertex_size(moon_parts);
        const uint32_t index_size = sphare::index_size(moon_parts);

        byte *_buffer =
            (byte *)mem.malloc_aligend_64(texture_size * sizeof(byte));
        get_res_iapetus_jpg(_buffer);

        obj_3d_vertex *vertexes = (obj_3d_vertex *)mem.malloc_aligend_64(
            vertex_size * sizeof(obj_3d_vertex));
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
        const uint32_t texture_size = get_res_mimas_jpg_size();
        const uint32_t vertex_size = sphare::vertex_size(moon_parts);
        const uint32_t index_size = sphare::index_size(moon_parts);

        byte *_buffer =
            (byte *)mem.malloc_aligend_64(texture_size * sizeof(byte));
        get_res_mimas_jpg(_buffer);

        obj_3d_vertex *vertexes = (obj_3d_vertex *)mem.malloc_aligend_64(
            vertex_size * sizeof(obj_3d_vertex));
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
        const uint32_t texture_size = get_res_uranus_jpg_size();
        const uint32_t vertex_size = sphare::vertex_size(planet_parts);
        const uint32_t index_size = sphare::index_size(planet_parts);

        byte *_buffer =
            (byte *)mem.malloc_aligend_64(texture_size * sizeof(byte));
        get_res_uranus_jpg(_buffer);

        obj_3d_vertex *vertexes = (obj_3d_vertex *)mem.malloc_aligend_64(
            vertex_size * sizeof(obj_3d_vertex));
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
        const uint32_t texture_size = get_res_neptune_jpg_size();
        const uint32_t vertex_size = sphare::vertex_size(planet_parts);
        const uint32_t index_size = sphare::index_size(planet_parts);

        byte *_buffer =
            (byte *)mem.malloc_aligend_64(texture_size * sizeof(byte));
        get_res_neptune_jpg(_buffer);

        obj_3d_vertex *vertexes = (obj_3d_vertex *)mem.malloc_aligend_64(
            vertex_size * sizeof(obj_3d_vertex));
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

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, nullptr,
                                nullptr);
    glfwMakeContextCurrent(m_window);

    glewExperimental = GL_TRUE;
    GL_CHECK;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        exit(1);
    } else {
        GL_CHECK;
        if (!GLEW_VERSION_3_3) {
            fprintf(stderr, "OpenGL 3.3 is not supported\n");
            exit(1);
        }
    }
    GL_CHECK;
    glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetCursorPos(m_window, halfWindowWidth, halfWindowHeight);

    Viewport::update({0, 0, windowWidth, windowHeight});
    GL_CHECK;
    glClearColor(0.02f, 0.03f, 0.03f, 1.0f);

    glClearDepth(1.0f);
    glDepthFunc(GL_LESS);
    GL_CHECK;
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    GL_CHECK;
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    GL_CHECK;
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    buffer_store::inti();
    init_resources();
    GL_CHECK;
    {
        ShaderSource _solar_shaders[2]{
            {VERTEX, get_res_object_differed_geom_pass_vert_glsl()},
            {FRAGMENT, get_res_object_differed_geom_pass_frag_glsl()}};
        m_geom_pass_program.program_id = Program::compile(_solar_shaders, 2);

        UniformBuffer<4> uni(m_geom_pass_program.program_id);
        m_geom_pass_program.projection_id = uni["projectionMatrix"_h];
        m_geom_pass_program.view_id = uni["viewMatrix"_h];
        m_geom_pass_program.model_id = uni["modelMatrix"_h];

        glUseProgram(m_geom_pass_program.program_id);
        Program::set_uniform(uni["texture_diffuse"_h], 0);
        glUseProgram(0);
    }
    GL_CHECK;
    {
        ShaderSource _solar_shaders[2]{{VERTEX, get_res_object_vert_glsl()},
                                       {FRAGMENT, get_res_object_frag_glsl()}};
        m_normal_render.program_id = Program::compile(_solar_shaders, 2);

        UniformBuffer<4> uni(m_normal_render.program_id);
        m_normal_render.projection_id = uni["projectionMatrix"_h];
        m_normal_render.view_id = uni["viewMatrix"_h];
        m_normal_render.model_id = uni["modelMatrix"_h];

        glUseProgram(m_normal_render.program_id);
        Program::set_uniform(uni["texture_diffuse"_h], 0);
        glUseProgram(0);
    }
    GL_CHECK;
    {
        ShaderSource _solar_shaders[2]{
            {VERTEX, get_res_light_pass_vert_glsl()},
            {FRAGMENT, get_res_light_pass_frag_glsl()}};
        m_light_pass_program.program_id = Program::compile(_solar_shaders, 2);


        UniformBuffer<5> uni(m_light_pass_program.program_id);
        m_light_pass_program.model_id = uni["modelMatrix"_h];
        m_light_pass_program.pix_size_id = uni["pix_size"_h];
        m_light_pass_program.render_type_id = uni["render_type"_h];
        m_light_pass_program.camera_position_id = uni["camera_position"_h];

        glUseProgram(m_light_pass_program.program_id);
        Program::set_uniform(uni["position_texture"_h], 0);
        Program::set_uniform(uni["normal_texture"_h], 1);
        Program::set_uniform(uni["colour_texture"_h], 2);
        glUseProgram(0);
    }
    GL_CHECK;
    init_home_solar_system(m_solar_system, m_geom_pass_program.model_id);
    GL_CHECK;
    {
        ShaderSource text_[2]{{VERTEX, get_res_text_vert_glsl()},
                              {FRAGMENT, get_res_text_frag_glsl()}};

        Text::init(text_, 2, "projectionMatrix"_h, "modelMatrix"_h, "colour"_h);
    }
    GL_CHECK;
    {

        const uint32_t ttf_size = get_res_freesans_ttf_size();
        byte ttf_buffer[ttf_size];

        get_res_freesans_ttf(ttf_buffer);
        m_text = {ttf_buffer, ttf_size, 32};
        m_text.setColour(colour(150, 160, 170, 255));
        glDebugger::init(Text(ttf_buffer, ttf_size, 24));
    }

    GL_CHECK;
    int sample_count;
    glGetIntegerv(GL_MAX_SAMPLES, &sample_count);
    m_gbuffer.init(Viewport::width(), Viewport::height(), sample_count);
    m_temp.init(Viewport::width(), Viewport::height());

    GL_CHECK;
    {
        BitsMemory mem(1024 * 1024);
        byte *_area =
            (byte *)mem.malloc_aligend_64(get_res_smaa_area_data_size());
        byte *_search =
            (byte *)mem.malloc_aligend_64(get_res_smaa_search_data_size());

        get_res_smaa_area_data(_area);
        get_res_smaa_search_data(_search);
        GL_CHECK;
        m_smaa.init(get_res_smaa_shader(), _area, _search);
    }

    m_screen.init();
    GL_CHECK;
    glfwSwapInterval(2);

    deinit_resources();
}

void glProgram::exec() {
    Clock::update();
    uint32_t frame = 0;
    uint64_t start = Clock::now();
    sprintf(m_frameRateBuffer, "FPS: %.1f", m_frameRate);
    do {
        glfwPollEvents();
        Mouse::update(m_window);
        handle_input(m_input.poll(m_window));
        render();
        Clock::update();
        if (++frame % 90 == 0) {
            m_frameRate = 90000000000.f / (Clock::now() - start);
            start = Clock::now();
            PROF("Creating frame rate text");
            sprintf(m_frameRateBuffer, "FPS: %.1f", m_frameRate);
        }
        //		if (frame == 1000)
        //			return;

        if (glfwGetKey(m_window, GLFW_KEY_0) == GLFW_PRESS) {
            render_type = 0;
        } else if (glfwGetKey(m_window, GLFW_KEY_1) == GLFW_PRESS) {
            render_type = 1;
        } else if (glfwGetKey(m_window, GLFW_KEY_2) == GLFW_PRESS) {
            render_type = 2;
        } else if (glfwGetKey(m_window, GLFW_KEY_3) == GLFW_PRESS) {
            render_type = 3;
        } else if (glfwGetKey(m_window, GLFW_KEY_4) == GLFW_PRESS) {
            render_type = 4;
        } else if (glfwGetKey(m_window, GLFW_KEY_5) == GLFW_PRESS) {
            render_type = 5;
        } else if (glfwGetKey(m_window, GLFW_KEY_6) == GLFW_PRESS) {
            render_type = 6;
        } else if (glfwGetKey(m_window, GLFW_KEY_7) == GLFW_PRESS) {
            render_type = 7;
        } else if (glfwGetKey(m_window, GLFW_KEY_8) == GLFW_PRESS) {
            render_type = 8;
        } else if (glfwGetKey(m_window, GLFW_KEY_9) == GLFW_PRESS) {
            render_type = 9;
        }

    } while (glfwGetKey(m_window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             glfwWindowShouldClose(m_window) == 0);
}

void glProgram::render() {
    PROF("Render loop iteration");
    {
        GL_CHECK;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        {
            PROF("Setup before render");
            m_camera.update();
        }
        GL_CHECK;
        //				normal_pass();
        geom_pass();
        GL_CHECK;
        light_pass();
        GL_CHECK;
        {
            PROF("Rendering text");
            Text::beginRender();
            m_text.render(m_frameRateBuffer, 20, 40);
            //			glDebugger::flush();
            Text::endRender();
        }

        //		LOG("Hello %s", "world");
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

void glProgram::handle_input(const control &ctl) {
    if (ctl.val & ROT_X) {
        m_camera.rotateHorizontal(ctl.delta[0]);
    }

    if (ctl.val & ROT_Y) {
        m_camera.rotateVertical(ctl.delta[1]);
    }

    if (ctl.val & MOVE_X) {
        m_camera.move(ctl.delta[2]);
    }

    if (ctl.val & MOVE_Y) {
        m_camera.strafe(ctl.delta[3]);
    }

    if (ctl.val & ZOOM) {
        m_camera.zoom(ctl.delta[4]);
    }
}

void glProgram::geom_pass() {
    PROF("Geometry pass");
    GL_CHECK;

    //	glEnable(GL_CULL_FACE);
    //	glCullFace(GL_BACK);

    glUseProgram(m_geom_pass_program.program_id);
    m_gbuffer.begin_geom_pass();
    GL_CHECK;
    Program::set_uniform(m_geom_pass_program.projection_id,
                         m_camera.projection());
    Program::set_uniform(m_geom_pass_program.view_id, m_camera.view());
    GL_CHECK;
    m_solar_system.prepare();
    m_solar_system.render();
    GL_CHECK;
    {
        PROF("Performing blit");
        m_gbuffer.end_geom_pass();
        GL_CHECK;
    }

    //	glDisable(GL_CULL_FACE);
}

void glProgram::light_pass() {

    PROF("Light pass");
//    m_temp.begin_write();
    GL_CHECK;
    glUseProgram(m_light_pass_program.program_id);
    m_gbuffer.begin_render_pass();
    GL_CHECK;
    Program::set_uniform(m_light_pass_program.render_type_id, render_type);
    GL_CHECK;
    m_screen.render();
    GL_CHECK;
    m_gbuffer.end_render_pass();
    GL_CHECK;
//    m_temp.end_write();
    GL_CHECK;
//    m_smaa.run(m_temp.colour_buffer_id());
    GL_CHECK;
}

void glProgram::normal_pass() {
    glUseProgram(m_normal_render.program_id);

    Program::set_uniform(m_normal_render.projection_id, m_camera.projection());
    Program::set_uniform(m_normal_render.view_id, m_camera.view());
    GL_CHECK;
    m_solar_system.prepare();
    m_solar_system.render();
}

ScreenRender::~ScreenRender() { mesh_3d::destroy(m_mesh_id); }

void ScreenRender::init() {
    uv_3d_vertex vertexes[4]{{{-1.f, 1.f, 0.f}, {0.f, 1.f}},
                             {{-1.f, -1.f, 0.f}, {0.f, 0.f}},
                             {{1.f, -1.f, 0.f}, {1.f, 0.f}},
                             {{1.f, 1.f, 0.f}, {1.f, 1.f}}};
    uint32_t indexes[6]{0, 1, 2, 2, 3, 0};

    m_mesh_id = mesh_3d::create(vertexes, 4, indexes, 6);

    GL_CHECK;
}

void ScreenRender::render() {
    GL_CHECK;
    mesh_3d::render_geometry(m_mesh_id, 6);
    GL_CHECK;
}
