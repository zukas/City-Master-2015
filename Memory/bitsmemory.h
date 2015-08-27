#ifndef BITSMEMORY_H
#define BITSMEMORY_H
#include <stddef.h>

class BitsMemory final{
	private:
		struct manager *mem_manager;
	public:
		BitsMemory(size_t size);
		~BitsMemory();

		void *malloc(size_t size);
		void *malloc_aligend_16(size_t size);
		void *malloc_aligend_32(size_t size);
		void *malloc_aligend_64(size_t size);
		void clear();
};


#endif // BITSMEMORY_H
