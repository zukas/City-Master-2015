#ifndef TEXTURECOLLECTION2D_H
#define TEXTURECOLLECTION2D_H

#include "types.h"

struct texture2d
{
		GLID texture_id { 0 };
		GLID sampler_id { 0 };
};

class TextureCollection2D
{
	private:
		static texture2d g_textures[50];

	public:
		static GLID create_dds_from_memory(byte * buffer, size_t size);


		static void destroy(GLID texture_id);
		static void bind(GLID texture_id, GLID unit);
};

#endif // TEXTURECOLLECTION2D_H
