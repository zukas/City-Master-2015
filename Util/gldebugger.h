#ifndef GLDEBUGGER_H
#define GLDEBUGGER_H

#include <functional>
#include "text.h"

class glDebugger
{
		struct message
		{
				char *text;
				message *next;
		};
	private:
		message *m_first { nullptr };
		Text m_text;

	private:
		glDebugger();
		static glDebugger &get();

	public:
		static constexpr int log_size { 2048 };

	public:
		static void clear();
		static void inspect(const char *file, const char *function, int line);
		static void log(const char *format, ...);
		static void flush();
		static void init(Text &&text);
};

#define GLCLEAR() glDebugger::clear();
#define GLCHECK() glDebugger::inspect(__FILE__, __func__, __LINE__)
#define LOG(format, ...) glDebugger::log(format, __VA_ARGS__)


#endif //GLDEBUGGER_H
