#include "resources.h"
#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void create_output_dir(const char *directory);

int main(int argc, const char **argv) {

	if (argc > 2) {
		init();
        generate_resource_manager(&argv[1], argc - 3, argv[argc - 2], argv[argc - 1]);
		deinit();
	}

	return 0;
}

void create_output_dir(const char *directory) {
	struct stat st;
	if (stat(directory, &st) == -1) {
		mkdir(directory, 0777);
	}
}
