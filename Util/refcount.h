#ifndef REFCOUNT_H
#define REFCOUNT_H
#include "types.h"
#include <mutex>
#include <map>

class RefCount
{
    private:
		 std::map<uint32_t, int > m_counter;
         std::mutex m_lock;

    public:
		void operator + (uint32_t val);
		int operator - (uint32_t val);
};

#endif // REFCOUNT_H
