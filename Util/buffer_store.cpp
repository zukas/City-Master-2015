#include "buffer_store.h"
#include <GL/glew.h>
#include <GL/gl.h>

constexpr uint32_t _num{256};
constexpr uint32_t _size{3};

uint32_t _buffers[_num * _size];

void buffer_store::inti() {
    for (uint32_t i = 0; i < _num * _size; ++i) {
        _buffers[i] = 0;
    }
}

uint32_t *buffer_store::get(uint32_t id) { return &_buffers[id * _size]; }
