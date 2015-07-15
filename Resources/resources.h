#ifndef RESOURCES_H
#define RESOURCES_H

#include <cstddef>

struct filesname
{
		char name[256];
};

void load_resource(char *out_filename, const char *file, const char * out_dir);

void generate_resources(filesname *out_files, const char **files, size_t size, const char *out_dir);

#endif // RESOURCES_H
