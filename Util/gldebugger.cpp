#include "gldebugger.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <stdio.h>
#include <cstdarg>
#include <cstring>

glDebugger::glDebugger()
{
}

glDebugger &glDebugger::get()
{
	static glDebugger debug;
	return debug;
}

void glDebugger::clear()
{
	glGetError();
}

void glDebugger::inspect(const char *file, const char *function, int line)
{
	auto error = glGetError();
	if(error != GL_NO_ERROR)
	{
		log("%s - %s(%d) Error: %d", file, function, line, error);
	}
}

void glDebugger::log(const char *format, ...)
{
	char buffer [log_size];
	std::va_list args;
	va_start (args, format);
	auto size = std::vsnprintf(buffer, log_size, format, args);
	va_end (args);

	auto &obj = glDebugger::get();
	obj.m_first = new message { new char[size + 1] { 0 }, obj.m_first };
	std::strncpy(obj.m_first->text,	buffer, size + 1);
}

void glDebugger::flush()
{
	auto &obj = glDebugger::get();
	auto viewport = obj.m_text.program().getViewport();
	auto next = obj.m_first;
	for(int i = 0; i < viewport.h - 50 && next; i += (obj.m_text.lineHeight() + 5))
	{
		obj.m_text.render(next->text, { 0.1, 0.1, 0.1, 1.0 }, viewport.w - 400, viewport.h - 25 - i);
		next = next->next;
	}
}

void glDebugger::init(Text &&text)
{
	auto &obj = glDebugger::get();
	obj.m_text = std::move(text);
	obj.m_text.program().use();
}
