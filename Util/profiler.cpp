#include "profiler.h"
#include "utils.h"

#include <time.h>
#include <stdio.h>

struct prof_id {
    uint32_t sid_file{0};
    uint32_t sid_func{0};
    uint32_t line{0};
};

struct prof_info {
    uint32_t counter{0};
    uint64_t work_data{0};
    uint64_t duration{0};
};

prof_id __profiler_ids[1000];
prof_info __profiler_data[1000];
const char *__function_names[1000];
const char *__file_names[1000];

uint16_t __current_index{0};

void begin(uint16_t idx) {

    struct timespec tp;
    clock_gettime(CLOCK_MONOTONIC_RAW, &tp);

    prof_info &info = __profiler_data[idx];
    ++info.counter;
    info.work_data = tp.tv_sec * 1000000000 + tp.tv_nsec;
}

void end(uint16_t idx) {

    struct timespec tp;
    clock_gettime(CLOCK_MONOTONIC_RAW, &tp);

    prof_info &info = __profiler_data[idx];
    info.duration +=
        ((tp.tv_sec * 1000000000 + tp.tv_nsec) - info.work_data);
}

Profiler::Profiler(const char *file, const char *func, unsigned line) {
    uint32_t file_id = crypto::sid(file);
    uint32_t func_id = crypto::sid(func);

    const uint16_t size = __current_index;
    for (uint16_t i = 0; i < size; ++i) {
        prof_id &tmp_ = __profiler_ids[i];
        if (tmp_.sid_file == file_id && tmp_.sid_func == func_id && tmp_.line == line) {
            m_idx = i;
            begin(m_idx);
            break;
        }
    }
    if (m_idx < 0) {
        ++__current_index;
        m_idx = size;
        __function_names[m_idx] = func;
        __file_names[m_idx] = file;
        prof_id &tmp_ = __profiler_ids[m_idx];
        tmp_.sid_file = file_id;
        tmp_.sid_func = func_id;
        tmp_.line = line;
        begin(m_idx);
    }
}

Profiler::~Profiler() { end(m_idx); }


void print_profiler_results()
{
    const uint16_t size = __current_index;
    for(uint16_t i = 0; i < size; ++i)
    {
        const prof_info &info = __profiler_data[i];
        printf("%s::%s: call count: %u, total duration: %lu(ms), avg duration: %.4f(ms)\n", __file_names[i], __function_names[i], info.counter, info.duration / 1000000, (double)(info.duration / 1000000) / info.counter);
    }
}
