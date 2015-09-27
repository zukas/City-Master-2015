#ifndef TEMPBUFFER_H
#define TEMPBUFFER_H

#include "types.h"

class TempBuffer {
	public:
		enum type_t {
			RGBA,
			RGB,
			RGB16F
		};

  private:
	uint32_t m_frame_buffer;
	uint32_t m_colour_buffer;
  public:
	TempBuffer();
	void init(uint32_t width, uint32_t height, type_t type);
	void begin_write();
	void end_write();
	uint32_t colour_buffer_id() const;

};

#endif // TEMPBUFFER_H
