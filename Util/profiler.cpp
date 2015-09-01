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

prof_id __profiler_ids[1024];
prof_info __profiler_data[1024];
const char *__function_names[1024];
const char *__file_names[1024];
const char *__given_names[1024];

uint16_t __current_index{0};

uint64_t __call_stack[2048]{0};
int32_t __call_stack_idx{-1};

void begin(uint16_t idx) {

    struct timespec tp;
    clock_gettime(CLOCK_MONOTONIC_RAW, &tp);

    prof_info &info = __profiler_data[idx];
    ++info.counter;
    info.work_data = tp.tv_sec * 1000000000 + tp.tv_nsec;
	++__call_stack_idx;
}

void end(uint16_t idx) {

    struct timespec tp;
    clock_gettime(CLOCK_MONOTONIC_RAW, &tp);

	uint64_t pre_duration = __call_stack[__call_stack_idx + 1];
	__call_stack[__call_stack_idx + 1] = 0;

	prof_info &info = __profiler_data[idx];

	uint64_t duration =
		((tp.tv_sec * 1000000000 + tp.tv_nsec) - info.work_data) - pre_duration;
	__call_stack[__call_stack_idx--] += duration;

	info.duration += duration;
}

Profiler::Profiler(const char *name, const char *file, const char *func,
				   unsigned line) {
	const uint32_t file_id = crypto::sid(file);
	const uint32_t func_id = crypto::sid(func);

    const uint16_t size = __current_index;
    for (uint16_t i = 0; i < size; ++i) {
		const prof_id tmp_ = __profiler_ids[i];
		if (tmp_.sid_file == file_id && tmp_.sid_func == func_id &&
			tmp_.line == line) {
            m_idx = i;
            break;
        }
    }
    if (m_idx < 0) {
        ++__current_index;
        m_idx = size;
        __function_names[m_idx] = func;
        __file_names[m_idx] = file;
		__given_names[m_idx] = name;
        prof_id &tmp_ = __profiler_ids[m_idx];
        tmp_.sid_file = file_id;
        tmp_.sid_func = func_id;
        tmp_.line = line;
    }

	begin(m_idx);
}

Profiler::~Profiler() { end(m_idx); }

void print_profiler_results() {
    const uint16_t size = __current_index;
	for (uint16_t i = 0; i < size; ++i) {
		const prof_info info = __profiler_data[i];
		printf("%80s, %8u, %18.6f, %12.6f\n", __given_names[i], info.counter,
			   double(info.duration) / 1000000.f,
			   (double)(info.duration / 1000000.f) / info.counter);
    }
}
