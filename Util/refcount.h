#ifndef REFCOUNT_H
#define REFCOUNT_H
#include "types.h"
#include <mutex>
#include <map>

class RefCount
{
    private:
         std::map<GLID, int > m_counter;
         std::mutex m_lock;

    public:
        void operator + (GLID val);
        int operator - (GLID val);
};

#endif // REFCOUNT_H
