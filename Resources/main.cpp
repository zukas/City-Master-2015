#include "resources.h"
#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void create_output_dir(const char *directory);

int main(int argc, const char **argv) {

  if (argc > 2) {

    filesname *buffer = new filesname[argc - 2];
    generate_resources(buffer, &argv[1], argc - 2, argv[argc - 1]);

//    for (int i = 0; i < argc - 2; ++i) {
//      printf("%s\r\n", buffer[i].name);
//    }
////    printf("%s\r\n", buffer[argc - 3].name);
//    fflush(stdout);

    //	char cmd[8192];
    //	size_t size = 8192;
    //	size_t offset = 0;
    //	char **data = new char *[argc - 2 + 3];
    //	offset +=
    //		snprintf(&cmd[offset], size - offset, "/usr/bin/c++ -O2
    //-std=c++11
    //-c");
    //	for (int i = 0; i < argc - 2; ++i) {
    //	  offset += snprintf(&cmd[offset], size - offset, " %s",
    // buffer[i].name);
    //	}

    //	printf("CMD: %s\n", cmd);
    //	fflush(stdout);
    //	FILE *out = popen(cmd, "r");

    //	if (out) {
    //	  char out_buf[2048];
    //	  while (fgets(out_buf, 2048, out)) {
    //		printf("%s\n", out_buf);
    //	  }
    //	  pclose(out);
    //	} else {
    //	  printf("Error while compiling");
    //	}

    //	free(data);
    delete [] buffer;
  }

  return 0;
}

void create_output_dir(const char *directory) {
  struct stat st;
  if (stat(directory, &st) == -1) {
    mkdir(directory, 0777);
  }
}
