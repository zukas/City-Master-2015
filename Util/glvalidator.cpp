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
		printf("OpenGL encountered a GL_INVALID_ENUM error: %s(%u)\n", function,
			   line);
		break;
	case GL_INVALID_VALUE:
		printf("OpenGL encountered a GL_INVALID_VALUE error: %s(%u)\n",
			   function, line);
		break;
	case GL_INVALID_OPERATION:
		printf("OpenGL encountered a GL_INVALID_OPERATION error: %s(%u)\n",
			   function, line);
		break;
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		printf("OpenGL encountered a GL_INVALID_FRAMEBUFFER_OPERATION error: "
			   "%s(%u)\n",
			   function, line);
		break;
	case GL_OUT_OF_MEMORY:
		printf("OpenGL encountered a GL_OUT_OF_MEMORY error: %s(%u)\n",
			   function, line);
		break;
	default:
		printf("OpenGL encountered a %d error: %s(%u)\n", error, function,
			   line);
		break;
	}
}
