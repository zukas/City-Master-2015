#ifndef GLVALIDATOR_H
#define GLVALIDATOR_H

void check_gl_error(const char *file, unsigned int line, const char *function);

#ifdef DEBUG_BUILD
#define GL_CHECK check_gl_error(__FILE__, __LINE__, __func__)
#else
#define GL_CHECK
#endif

#endif // GLVALIDATOR_H
