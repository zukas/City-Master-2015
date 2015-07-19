#ifndef UTILS_H
#define UTILS_H

#include <cstddef>
#include <cstdint>

constexpr void *ptr_add(void *ptr, size_t size) {
  return reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(ptr) + size);
}

constexpr void *ptr_sub(void *ptr, size_t size) {
  return reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(ptr) - size);
}

constexpr size_t align_block_16(size_t size){
	return (size + 15) & ~ 0x0F;
}

constexpr size_t align_block_32(size_t size){
	return ((size + 31) & ~ 0x1F);
}

constexpr size_t align_block_64(size_t size){
	return ((size + 63) & ~ 0x3F);
}

#endif // UTILS_H
