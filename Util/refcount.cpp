#include "refcount.h"
#include <algorithm>

void RefCount::operator + (uint32_t val)
{
    std::lock_guard<std::mutex > _lock(m_lock);
    ++m_counter[val];
}

int RefCount::operator -(uint32_t val)
{
    std::lock_guard<std::mutex > _lock(m_lock);
    int res = --m_counter[val];
    if(res == 0) m_counter.erase(val);
    return res;
}
