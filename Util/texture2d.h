#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include "types.h"

class Texture2D {
  private:
	GLID m_texture_id{0};
	GLID m_sampler_id{0};

  public:
	Texture2D() = default;
	Texture2D(Texture2D &&) = default;
	~Texture2D();
	void init_dds_from_memory(byte *buffer, uint32_t size);
	void bind(GLID unit) const;

	Texture2D &&move();

	Texture2D &operator=(Texture2D &&) = default;
};

template <uint32_t __S> class Texture2DArray {
  private:
	Texture2D m_array[__S];

  public:
	Texture2DArray() = default;
	Texture2DArray(Texture2DArray &&other) {
		for (uint32_t i = 0; i < __S; ++i) {
			m_array[i] = other.m_array[i].move();
		}
	}
	constexpr uint32_t size() const { return __S; }
	Texture2DArray<__S> move() { return ::move(*this); }

	Texture2D &operator[](uint32_t idx) { return m_array[idx]; }
	const Texture2D &operator[](uint32_t idx) const { return m_array[idx]; }

	Texture2DArray &operator=(Texture2DArray &&other) {
		for (uint32_t i = 0; i < __S; ++i) {
			m_array[i] = other.m_array[i].move();
		}
		return *this;
	}
};

#endif // TEXTURE2D_H
