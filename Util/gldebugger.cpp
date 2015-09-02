#include "gldebugger.h"
#include "viewport.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

glDebugger::glDebugger() {}

glDebugger &glDebugger::get() {
	static glDebugger debug;
	return debug;
}

void glDebugger::clear() { glGetError(); }

void glDebugger::inspect(const char *file, const char *function, int line) {
	auto error = glGetError();
	if (error != GL_NO_ERROR) {
		log("%s - %s(%d) Error: %d", file, function, line, error);
	}
}

void glDebugger::log(const char *format, ...) {
	char buffer[log_size];
    va_list args;
	va_start(args, format);
    auto size = vsnprintf(buffer, log_size, format, args);
	va_end(args);

	auto &obj = glDebugger::get();
	obj.m_first = new message{new char[size + 1]{0}, obj.m_first};
    strncpy(obj.m_first->text, buffer, size + 1);
}

void glDebugger::flush() {
	auto &obj = glDebugger::get();
	const rect viewport = Viewport::viewport();
	uint32_t vertical_offset = 0;
	auto next = obj.m_first;
	for (int i = 0; i < viewport.h - 50 && next; i += (vertical_offset + 5)) {
		vertical_offset +=
			obj.m_text.render(next->text, viewport.w - 600, 25 + i);
		next = next->next;
	}
}

void glDebugger::init(Text &&text) {
	auto &obj = glDebugger::get();
	obj.m_text = move(text);
}
