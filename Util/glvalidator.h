#ifndef GLVALIDATOR_H
#define GLVALIDATOR_H

void check_gl_error(const char *file, unsigned int line, const char *function);
void check_gl_framebuffer_error(const char *file, unsigned int line, const char *function);

#ifdef DEBUG_BUILD
#define GL_CHECK check_gl_error(__FILE__, __LINE__, __func__)
#define GL_CHECK_FB check_gl_framebuffer_error(__FILE__, __LINE__, __func__)
#else
#define GL_CHECK
#define GL_CHECK_FB
#endif

#endif // GLVALIDATOR_H
