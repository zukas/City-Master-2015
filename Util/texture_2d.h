#ifndef TEXTURE_2D_H
#define TEXTURE_2D_H

#include "types.h"

namespace texture_2d {

    uint32_t create_dss_from_memory(byte *buffer, uint32_t size);

    uint32_t create_jpg_from_memory(byte *buffer, uint32_t size);

    uint32_t create_ttf_from_memory(byte *buffer, uint32_t width,
										   uint32_t height);

    uint32_t create_sampler();

    void destroy(uint32_t texture_id);

    void bind(uint32_t texture_id, uint32_t unit = 0);

    void bind_sampler(uint32_t sampler_id, uint32_t unit = 0);

    void bind(const uint32_t *texture_ids, uint32_t size, uint32_t start_unit = 0);

    void unbind(uint32_t unit = 0);

    void unbind_sampler(uint32_t unit = 0);

    void save(const char *filename, uint32_t texture_id);
};

#endif // TEXTURE_2D_H
