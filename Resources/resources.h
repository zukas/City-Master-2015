#ifndef RESOURCES_H
#define RESOURCES_H

#include <cstddef>

void init();

void deinit();

void load_resource(char *out_header, const char *file, const char * out_dir);

void generate_resources(const char **files, size_t size, const char *out_dir);

#endif // RESOURCES_H
