#include "glprogram.h"
#include "Memory/bitsmemory.h"

#include "Common/str_hash.h"

#include "Util/clock.h"
#include "Util/mouse.h"
#include "Util/shapes.h"
#include "Util/gldebugger.h"
#include "Util/profiler.h"
#include "Util/viewport.h"
#include "Util/texture2dcollection.h"
#include "Util/mesh3dcollection.h"

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
        calc_size = size_scaling(40.f, in.size);
        calc_distance = distance_scaling(200.f, in.distance);
        break;
    case MOON:
        calc_size = size_scaling(255.f, in.size);
        calc_distance = distance_scaling(2000.f, in.distance);
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
		Shader _solar_shaders[2]{
			{VERTEX, get_res_object_differed_geom_pass_vert_glsl()},
			{FRAGMENT, get_res_object_differed_geom_pass_frag_glsl()}};
		m_geom_pass_program.program_id =
			ProgramCompiler::compileProgram(_solar_shaders, 2);

		const int _uni_size = 4;
		uint32_t _solar_uni[_uni_size];
		ProgramCompiler::resolveUniforms(m_geom_pass_program.program_id,
										 _solar_uni, _uni_size);
		m_geom_pass_program.projection_id =
			Uniforms::getUniformId(_solar_uni, _uni_size, "projectionMatrix"_h);
		m_geom_pass_program.view_id =
			Uniforms::getUniformId(_solar_uni, _uni_size, "viewMatrix"_h);
		m_geom_pass_program.model_id =
			Uniforms::getUniformId(_solar_uni, _uni_size, "modelMatrix"_h);

		glUseProgram(m_geom_pass_program.program_id);
		Uniforms::setUniform(
			Uniforms::getUniformId(_solar_uni, _uni_size, "texture_diffuse"_h),
			0);
		glUseProgram(0);
	}

    {
		Shader _solar_shaders[2]{{VERTEX, get_res_light_pass_vert_glsl()},
								 {FRAGMENT, get_res_light_pass_frag_glsl()}};
		m_light_pass_program.program_id =
			ProgramCompiler::compileProgram(_solar_shaders, 2);

		const int _uni_size = 6;
        uint32_t _solar_uni[_uni_size];
		ProgramCompiler::resolveUniforms(m_light_pass_program.program_id,
										 _solar_uni, _uni_size);
		m_light_pass_program.model_id =
			Uniforms::getUniformId(_solar_uni, _uni_size, "modelMatrix"_h);
		m_light_pass_program.screen_size_id =
			Uniforms::getUniformId(_solar_uni, _uni_size, "screen_size"_h);
		m_light_pass_program.render_type_id =
			Uniforms::getUniformId(_solar_uni, _uni_size, "render_type"_h);
        m_light_pass_program.camera_position_id =
            Uniforms::getUniformId(_solar_uni, _uni_size, "camera_position"_h);
		glUseProgram(m_light_pass_program.program_id);
		Uniforms::setUniform(
			Uniforms::getUniformId(_solar_uni, _uni_size, "position_texture"_h),
			0);
		Uniforms::setUniform(
			Uniforms::getUniformId(_solar_uni, _uni_size, "normal_texture"_h),
			1);
		Uniforms::setUniform(
			Uniforms::getUniformId(_solar_uni, _uni_size, "colour_texture"_h),
			2);
		glUseProgram(0);
    }

	init_home_solar_system(m_solar_system, m_geom_pass_program.model_id);

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

    deinit_resources();

	m_gbuffer.init();
	m_screen.init();

	glfwSwapInterval(1);
}

void glProgram::exec() {
    Clock::update();
    uint32_t frame = 0;
    uint64_t start = Clock::now();
    do {
        glfwPollEvents();
		Mouse::update(m_window);
        handle_input(m_input.poll(m_window));
        render();
        Clock::update();
		if (++frame % 30 == 0) {
            m_frameRate = 30000000000.f / (Clock::now() - start);
            start = Clock::now();
        }
		//		if (frame == 1000)
		//			return;

        if(glfwGetKey(m_window, GLFW_KEY_0) == GLFW_PRESS) {
            render_type = 0;
        } else if(glfwGetKey(m_window, GLFW_KEY_1) == GLFW_PRESS) {
            render_type = 1;
        } else if(glfwGetKey(m_window, GLFW_KEY_2) == GLFW_PRESS) {
            render_type = 2;
        } else if(glfwGetKey(m_window, GLFW_KEY_3) == GLFW_PRESS) {
            render_type = 3;
        }

    } while (glfwGetKey(m_window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             glfwWindowShouldClose(m_window) == 0);
}

void glProgram::render() {
    PROF("Render loop iteration");
    {
		//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        {
            PROF("Setup before render");
            m_camera.update();
        }
		//		m_gbuffer.begin_frame();
		geom_pass();

		//		glEnable(GL_STENCIL_TEST);
		//		cencil_pass();
		//		glDisable(GL_STENCIL_TEST);
		light_pass();

		//		m_gbuffer.begin_final_render();
		//		glBlitFramebuffer(0, 0, Viewport::width(),
		// Viewport::height(), 0, 0,
		//						  Viewport::width(),
		// Viewport::height(),
		//						  GL_COLOR_BUFFER_BIT,
		// GL_LINEAR);
		{
			PROF("Creating frame rate text");
			char buf[128];
			sprintf(buf, "FPS: %.2f", m_frameRate);
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

	glUseProgram(m_geom_pass_program.program_id);
	m_gbuffer.begin_geom_pass();

	Uniforms::setUniform(m_geom_pass_program.projection_id,
						 m_camera.projection());
	Uniforms::setUniform(m_geom_pass_program.view_id, m_camera.view());

	m_solar_system.prepare();
	m_solar_system.render();

	m_gbuffer.end_geom_pass();
}

void glProgram::light_pass() {

    glEnable(GL_BLEND);
	glUseProgram(m_light_pass_program.program_id);
	m_gbuffer.begin_render_pass();

    Uniforms::setUniform(m_light_pass_program.model_id, glm::mat4(1.f));
	Uniforms::setUniform(m_light_pass_program.screen_size_id,
						 glm::vec2(Viewport::width(), Viewport::height()));
    Uniforms::setUniform(m_light_pass_program.render_type_id, render_type);
    Uniforms::setUniform(m_light_pass_program.camera_position_id, m_camera.position());

	m_screen.render();
	m_gbuffer.end_render_pass();
    glDisable(GL_BLEND);
}

ScreenRender::~ScreenRender() { Mesh3DCollection::destroy(m_mesh_id); }

void ScreenRender::init() {
	glm::vec3 vertexes[4]{
		{-1.f, 1.f, 0.f}, {-1.f, -1.f, 0.f}, {1.f, -1.f, 0.f}, {1.f, 1.f, 0.f}};
	uint32_t indexes[6]{0, 1, 2, 2, 3, 0};

	m_mesh_id = Mesh3DCollection::create(vertexes, 4, indexes, 6);
}

void ScreenRender::render() { Mesh3DCollection::render_geometry(m_mesh_id, 6); }
