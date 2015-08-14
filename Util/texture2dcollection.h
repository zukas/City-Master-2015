#ifndef TEXTURE2DCOLLECTION_H
#define TEXTURE2DCOLLECTION_H

#include "types.h"

#ifndef MAX_TEXTURE_2D_SIZE
#define MAX_TEXTURE_2D_SIZE 64
#endif

#ifndef MAX_SAMPLER_SIZE
#define MAX_SAMPLER_SIZE 4
#endif

class Texture2DCollection {
  private:
    static uint32_t _texture_size;
    static uint32_t _sampler_size;

  public:
    Texture2DCollection() = delete;

    static void init(uint32_t texture_size, uint32_t sampler_size);
    static void destroy();
    static uint32_t texture_size();
    static uint32_t sampler_size();

    static void cleate_dss_from_memory(uint32_t index, byte *buffer,
                                       uint32_t size);
    static void bind(const uint32_t *texture_indexes, uint32_t size);
};

#endif // TEXTURE2DCOLLECTION_H
