#ifndef BITSMEMORY_H
#define BITSMEMORY_H
#include <cstddef>

class BitsMemory final{
	private:
		static struct manager *mem_manager;
	public:
		BitsMemory() = delete;
		static void init(size_t size);
		static void deinit();

		static void *malloc(size_t size);
		static void *malloc_aligend_16(size_t size);
		static void *malloc_aligend_32(size_t size);
		static void *malloc_aligend_64(size_t size);
		static void clear();

};

#endif // BITSMEMORY_H
