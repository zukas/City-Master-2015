#include "bitsmemory.h"
#include "Common/ptr_arithm.h"

#include <cassert>
#include <cstdlib>
#include <new>

struct alignas(32) manager {
	void *mem_block{nullptr};
	void *current_block{nullptr};
	size_t index;
	size_t size;
};

BitsMemory::BitsMemory() : mem_manager(nullptr) {}

BitsMemory::BitsMemory(size_t size) {
	void *tmp = ::malloc(size + sizeof(manager));
	mem_manager = new (tmp) manager();
	mem_manager->mem_block = mem_manager->current_block =
		ptr_add(tmp, sizeof(manager));
	mem_manager->size = size;
	mem_manager->index = 0;
}

BitsMemory::~BitsMemory() {
	assert(mem_manager != nullptr);
	::free(mem_manager);
	mem_manager = nullptr;
}

void *BitsMemory::malloc(size_t size) {
	void *res = mem_manager->current_block;
	mem_manager->index += size;
	assert(mem_manager->index < mem_manager->size);
	mem_manager->current_block = ptr_add(mem_manager->current_block, size);
	return res;
}

void *BitsMemory::malloc_aligend_16(size_t size) {
	return malloc(align_block_16(size));
}

void *BitsMemory::malloc_aligend_32(size_t size) {
	return malloc(align_block_32(size));
}

void *BitsMemory::malloc_aligend_64(size_t size) {
	return malloc(align_block_32(size));
}

void BitsMemory::clear() {
	mem_manager->current_block = mem_manager->mem_block;
	mem_manager->index = 0;
}

void BitsMemory::swap(BitsMemory &other) {
	manager *tmp = mem_manager;
	mem_manager = other.mem_manager;
	other.mem_manager = tmp;
}
