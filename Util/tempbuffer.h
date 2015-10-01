#ifndef TEMPBUFFER_H
#define TEMPBUFFER_H

#include "types.h"

class TempBuffer {
  private:
	uint32_t m_frame_buffer;
	uint32_t m_colour_buffer;
	uint32_t m_width;
	uint32_t m_height;
  public:
	TempBuffer();
	void init(uint32_t width, uint32_t height);
	void begin_write();
	void end_write();
	void copy_to_buffer(uint32_t buffer_id = 0);
	uint32_t colour_buffer_id() const;

};

#endif // TEMPBUFFER_H
