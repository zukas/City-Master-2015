#ifndef PROFILER_H
#define PROFILER_H

class Profiler
{
    int m_idx { -1 };
public:
	Profiler(const char *name, const char* file, const char *func, unsigned line);
    ~Profiler();
};

void print_profiler_results();

#define PP_CAT(a, b) PP_CAT_I(a, b)
#define PP_CAT_I(a, b) PP_CAT_II(~, a ## b)
#define PP_CAT_II(p, res) res

#define PROF(name) Profiler PP_CAT(__profile_func_at_, __COUNTER__)(name, __FILE__, __FUNCTION__,__LINE__)

#endif // PROFILER_H
