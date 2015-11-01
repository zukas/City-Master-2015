#ifndef BUFFER_STORE_H
#define BUFFER_STORE_H

#include "types.h"

namespace buffer_store {
void inti();
uint32_t * create(uint32_t id, uint32_t size);
const uint32_t * const get(uint32_t id);
}

#endif // BUFFER_STORE_H
