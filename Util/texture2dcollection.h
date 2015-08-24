#ifndef TEXTURE2DCOLLECTION_H
#define TEXTURE2DCOLLECTION_H

#include "types.h"

#ifndef MAX_TEXTURE_2D_SIZE
#define MAX_TEXTURE_2D_SIZE 256
#endif

class Texture2DCollection {
  private:
    static uint32_t _texture_size;

  public:
    Texture2DCollection() = delete;

	static void init(uint32_t texture_size);
    static void destroy();
    static uint32_t texture_size();

	static uint32_t create_dss_from_memory(byte *buffer, uint32_t size);

	static uint32_t create_ttf_from_memory(byte *buffer, uint32_t width,
										   uint32_t height);

	static void bind(uint32_t texture_id, uint32_t unit = 0);

	static void bind(const uint32_t *texture_ids, uint32_t size, uint32_t start_unit = 0);
};

#endif // TEXTURE2DCOLLECTION_H
