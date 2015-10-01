#include "glvalidator.h"

#include <stdio.h>

#include <GL/glew.h>
#include <GL/gl.h>

void check_gl_error(const char *file, unsigned int line, const char *function) {
	GLenum error = glGetError();
	switch (error) {
	case GL_NO_ERROR:
		break;
	case GL_INVALID_ENUM:
		printf("OpenGL encountered a GL_INVALID_ENUM error: %s %s(%u)\n", file,
			   function, line);
		break;
	case GL_INVALID_VALUE:
		printf("OpenGL encountered a GL_INVALID_VALUE error: %s %s(%u)\n", file,
			   function, line);
		break;
	case GL_INVALID_OPERATION:
		printf("OpenGL encountered a GL_INVALID_OPERATION error: %s %s(%u)\n",
			   file, function, line);
		break;
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		printf("OpenGL encountered a GL_INVALID_FRAMEBUFFER_OPERATION error: "
			   "%s %s(%u)\n",
			   file, function, line);
		break;
	case GL_OUT_OF_MEMORY:
		printf("OpenGL encountered a GL_OUT_OF_MEMORY error: %s %s(%u)\n", file,
			   function, line);
		break;
	default:
		printf("OpenGL encountered a %d error: %s %s(%u)\n", error, file,
			   function, line);
		break;
	}
}

void check_gl_framebuffer_error(const char *file, unsigned int line,
								const char *function) {
	GLenum error = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch (error) {
	case GL_FRAMEBUFFER_COMPLETE:
		break;
	case GL_FRAMEBUFFER_UNDEFINED:
		printf("OpenGL encountered a GL_FRAMEBUFFER_UNDEFINED error: %s %s(%u)\n",
			   file, function, line);
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		printf("OpenGL encountered a GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT "
			   "error: %s %s(%u)\n",
			   file, function, line);
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		printf("OpenGL encountered a "
			   "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT error: %s %s(%u)\n",
			   file, function, line);
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		printf(
			"OpenGL encountered a GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER error: "
			"%s %s(%u)\n",
			file, function, line);
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
		printf(
			"OpenGL encountered a GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER error: "
			"%s %s(%u)\n",
			file, function, line);
		break;
	case GL_FRAMEBUFFER_UNSUPPORTED:
		printf("OpenGL encountered a GL_FRAMEBUFFER_UNSUPPORTED error: "
			   "%s %s(%u)\n",
			   file, function, line);
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
		printf(
			"OpenGL encountered a GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE error: "
			"%s %s(%u)\n",
			file, function, line);
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
		printf("OpenGL encountered a GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS "
			   "error: %s %s(%u)\n",
			   file, function, line);
		break;
	default:
		printf("OpenGL encountered a %d error: %s %s(%u)\n", error, file, function,
			   line);
		break;
	}
}
