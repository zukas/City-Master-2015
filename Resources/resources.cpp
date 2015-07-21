#include "resources.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctype.h>
#include <new>

#include "templates.h"
#include "crc32.h"

void *buffer{nullptr};
size_t buffer_size{1024 * 1024 * 50};

constexpr void *ptr_add(void *ptr, size_t size) {
    return reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(ptr) + size);
}

enum Storage { BINERY, TEXT };

Storage get_storage_type(const char *filename) {
    Storage t = TEXT;
    char file_name[32]{'\0'};
    size_t length = strlen(filename);
    for (int i = length - 1; i >= 0; --i) {
        if (filename[i] == '.') {
            size_t k = 0;
            for (size_t j = i + 1; j < length; ++j, ++k) {
                char c = filename[j];
                if (isalnum(c) == 0) {
                    break;
                }
                file_name[k] = tolower(c);
            }
            break;
        }
    }

    const char *array[]{"jpg", "jpeg", "png", "tga", "dds", "ttf"};

    for (size_t i = 0; i < sizeof(array) / sizeof(const char *) && t == TEXT;
         ++i) {
        if (strcmp(array[i], file_name) == 0) {
            t = BINERY;
        }
    }
    return t;
}

void get_filename(char *buffer, size_t size, const char *file,
                  char delimeter = '/') {
    size_t length = strlen(file);
    for (int i = length - 1; i >= -1; --i) {
        if (i == -1 || file[i] == delimeter) {
            size_t k = 0;
            for (size_t j = i + 1; j < length && k < size; ++j, ++k) {
                buffer[k] = file[j];
            }
            buffer[k] = '\0';
            break;
        }
    }
}

void get_basename(char *buffer, size_t size, const char *file,
                  char delimeter = '/') {
    size_t length = strlen(file);
    for (int i = length - 1; i >= -1; --i) {
        if (i == -1 || file[i] == delimeter) {
            size_t k = 0;
            for (size_t j = i + 1; j < length && k < size; ++j, ++k) {
                if (file[i] == '.') {
                    buffer[k] = '\0';
                    break;
                }
                buffer[k] = file[j];
            }
            buffer[k] = '\0';
            break;
        }
    }
}

void get_path(char *buffer, size_t size, const char *file,
              char delimeter = '/') {
    const char *end = strrchr(file, delimeter);
    if (end) {
        size_t i;
        for (i = 0; i < size && &file[i] != end; ++i) {
            buffer[i] = file[i];
        }
        buffer[i] = '\0';
    }
}

void get_resource_name_upper(char *buffer, size_t size, const char *file,
                             char delimeter = '/') {
    buffer[0] = 'R';
    buffer[1] = 'E';
    buffer[2] = 'S';
    buffer[3] = '_';
    get_filename(&buffer[4], size - 4, file, delimeter);
    for (size_t i = 0, l = strlen(buffer); i < l; ++i) {
        char &c = buffer[i];
        c = toupper(c);
        if (isalnum(c) == 0) {
            c = '_';
        }
    }
}

void get_resource_name_lower(char *buffer, size_t size, const char *file,
                             char delimeter = '/') {
    buffer[0] = 'r';
    buffer[1] = 'e';
    buffer[2] = 's';
    buffer[3] = '_';
    get_filename(&buffer[4], size - 4, file, delimeter);
    for (size_t i = 0, l = strlen(buffer); i < l; ++i) {
        char &c = buffer[i];
        c = tolower(c);
        if (isalnum(c) == 0) {
            c = '_';
        }
    }
}

uint32_t file_hash(const char *filename) {
    uint32_t res{0};
    FILE *file = fopen64(filename, "rb");
    if (file) {
		size_t file_size = 0;
		size_t read_size = 0;
		size_t tmp_size = 0;
		char read_buffer[2048];
		fseek(file, 0, SEEK_END);
		file_size = ftell(file);
        rewind(file);

		while (!feof(file)) {
			tmp_size = fread(read_buffer, 1, sizeof(read_buffer), file);
			read_size += tmp_size;
			res = crc32_fast(read_buffer, tmp_size, res);
		}
		if (file_size != read_size) {
			res = 0;
		}

        fclose(file);
    }

    return res;
}

void load_resource(char *out_header, const char *file, const char *out_dir) {

    Storage type = get_storage_type(file);
    char name[128]{'\0'};
    get_resource_name_lower(name, 128, file);
    if (out_header != nullptr) {
        snprintf(out_header, 256, "%s.h", name);
    }

    FILE *tmp = fopen64(file, "rb");
    if (tmp) {
        fseeko64(tmp, 0, SEEK_END);
        long size = ftello64(tmp);
        rewind(tmp);
        size_t min_buffer_size = (size << 2) * sizeof(int);
        if (min_buffer_size >= buffer_size) {
            printf("File is bigger then the allocated buffer\n");
        }
        long buffer_offest = 0;
        if (type ==
            BINERY) { /*
                 buffer_offest +=
                         snprintf(&buffer[buffer_offest], buffer_size -
         buffer_offest,
                                          "#include \"%s.h\"\n"
                                          "#include <new>\n"
                                          "unsigned char *get_%s(void *buffer) {
         return new "
                                          "(buffer) unsigned char [%lu] {",
                                          name, name, size);

                 int c = fgetc(tmp);
                 buffer_offest += snprintf(&buffer[buffer_offest],
                                                                   buffer_size -
         buffer_offest, "%d", c);
                 while (tmp && !feof(tmp)) {
                         c = fgetc(tmp);
                         if (c != EOF) {
                                 buffer_offest +=
                                         snprintf(&buffer[buffer_offest],
                                                          buffer_size -
         buffer_offest, ",%d", c);
                         }
                 }

                 buffer_offest += snprintf(&buffer[buffer_offest],
                                                                   buffer_size -
         buffer_offest, "}; }\n");
         } else {
                 int c;
                 buffer_offest +=
                         snprintf(&buffer[buffer_offest], buffer_size -
         buffer_offest,
                                          "#include \"%s.h\"\nconst char*
         get_%s() { return \"",
                                          name, name);
                 while (tmp && !feof(tmp)) {
                         c = fgetc(tmp);
                         if (c == '\n') {
                                 buffer_offest +=
                                         snprintf(&buffer[buffer_offest],
                                                          buffer_size -
         buffer_offest, "\\n");
                         }
                         if (isprint(c) && c != EOF) {
                                 buffer_offest +=
                                         snprintf(&buffer[buffer_offest],
                                                          buffer_size -
         buffer_offest, "%c", c);
                         }
                 }

                 buffer_offest += snprintf(&buffer[buffer_offest],
                               buffer_size - buffer_offest, "\"; }\n");*/
        }

        fclose(tmp);

        char full_file_path[512]{'\0'};

        snprintf(full_file_path, 512, "%s/%s.cpp", out_dir, name);
        size_t original_hash = file_hash(full_file_path);
        size_t generated_hash = crc32_fast(buffer, buffer_offest, 0);

        if (original_hash != generated_hash) {
            printf("Generating sources for %s\n", file);
            tmp = fopen64(full_file_path, "wb");
            if (tmp) {
                printf("Updating %s\n", full_file_path);
                fwrite(buffer, 1, buffer_offest, tmp);
                fclose(tmp);
            }

            snprintf(full_file_path, 512, "%s/%s.h", out_dir, name);
            tmp = fopen64(full_file_path, "wb");
            if (tmp) {
                printf("Updating %s\n", full_file_path);
                char __tmp[256]{'\0'};
                strcpy(__tmp, name);

                size_t i;
                for (i = 0; i < strlen(__tmp); ++i) {
                    __tmp[i] = toupper(__tmp[i]);
                }
                __tmp[i] = '_';
                __tmp[i + 1] = 'H';

                fprintf(tmp, "#ifndef %s\n#define %s\n", __tmp, __tmp);

                if (type == BINERY) {
                    fprintf(tmp, "#include <cstddef>\nconstexpr size_t %s_size "
                                 "{ %lu };\n",
                            name, size);
                    fprintf(tmp, "unsigned char *get_%s(void*);\n#endif\n",
                            name);
                } else {
                    fprintf(tmp, "const char* get_%s();\n#endif\n", name);
                }

                fclose(tmp);
            }
        }
    }
}

constexpr const char *template_h_1{
    "#ifndef RESOURCEMANAGER_H\n"
    "#define RESOURCEMANAGER_H\n"
    "#include <cstddef>\n"
    "typedef void* (*malloc_function)(size_t);\n"
    "void set_malloc_function(malloc_function);\n"
    "enum class buffer_resource_t {\n"};
constexpr const char *template_h_2{"};\n"
                                   "enum class literal_resource_t {\n"};
constexpr const char *template_h_3{
    "};\n"
    "struct resource_buffer {\n"
    "unsigned char *buffer;\n"
    "size_t size;\n"
    "};\n"
    "resource_buffer load_buffer_resource(buffer_resource_t);\n"
    "const char *load_litteral_resource(literal_resource_t);\n"
    "#endif //RESOURCEMANAGER_H"};

constexpr const char *template_cpp_1{"#include \"resourcemanager.h\"\n"
                                     "#include <cstdlib>\n"};
constexpr const char *template_cpp_2{
    "malloc_function __g_malloc_func { malloc };\n"
    "void set_malloc_function(malloc_function func) { __g_malloc_func = func; "
    "}\n"
    "resource_buffer load_buffer_resource(buffer_resource_t type) {\n"
    "switch(type) {\n"};
constexpr const char *template_cpp_3{
    "default: return resource_buffer { nullptr, 0 };"
    "}\n"
    "}\n"
    "const char *load_litteral_resource(literal_resource_t type) {\n"
    "switch(type) {\n"};

constexpr const char *template_cpp_4{"default: return \"\";"
                                     "}\n"
                                     "}\n"};

void generate_resources(const char **files, size_t size, const char *out_dir) {

    char out_man_file[512]{'\0'};
    snprintf(out_man_file, 512, "%s/resourcemanager.h", out_dir);
    FILE *man_h = fopen64(out_man_file, "wb");
    snprintf(out_man_file, 512, "%s/resourcemanager.cpp", out_dir);
    FILE *man_cpp = fopen64(out_man_file, "wb");

    if (man_h && man_cpp) {
        fwrite(template_h_1, strlen(template_h_1), 1, man_h);
        fwrite(template_cpp_1, strlen(template_cpp_1), 1, man_cpp);
        size_t buff_res_cout{0};
        for (size_t i = 0; i < size; ++i) {
            if (get_storage_type(files[i]) == BINERY) {
                ++buff_res_cout;
            }
            char out_buffer[256]{'\0'};
            load_resource(out_buffer, files[i], out_dir);
            fprintf(man_cpp, "#include \"%s\"\n", out_buffer);
        }
        fwrite(template_cpp_2, strlen(template_cpp_2), 1, man_cpp);
        for (size_t i = 0, curr = 0, last = buff_res_cout - 1; i < size; ++i) {
            if (BINERY == get_storage_type(files[i])) {
                char up_res[256]{'\0'};
                char lo_res[256]{'\0'};
                get_resource_name_upper(up_res, 256, files[i]);
                get_resource_name_lower(lo_res, 256, files[i]);

                if (curr < last) {
                    fprintf(man_h, "%s,\n", up_res);
                } else {
                    fprintf(man_h, "%s\n", up_res);
                }

                fprintf(man_cpp,
                        "case buffer_resource_t::%s: return resource_buffer { "
                        "get_%s(__g_malloc_func(%s_size)), %s_size };\n",
                        up_res, lo_res, lo_res, lo_res);
                ++curr;
            }
        }
        fwrite(template_h_2, strlen(template_h_2), 1, man_h);
        fwrite(template_cpp_3, strlen(template_cpp_3), 1, man_cpp);
        for (size_t i = 0, curr = 0, last = size - 1 - buff_res_cout; i < size;
             ++i) {
            if (TEXT == get_storage_type(files[i])) {
                char up_res[256]{'\0'};
                char lo_res[256]{'\0'};
                get_resource_name_upper(up_res, 256, files[i]);
                get_resource_name_lower(lo_res, 256, files[i]);

                if (curr < last) {
                    fprintf(man_h, "%s,\n", up_res);
                } else {
                    fprintf(man_h, "%s\n", up_res);
                }

                fprintf(man_cpp,
                        "case literal_resource_t::%s: return get_%s();\n",
                        up_res, lo_res);
                ++curr;
            }
        }
        fwrite(template_h_3, strlen(template_h_3), 1, man_h);
        fwrite(template_cpp_4, strlen(template_cpp_4), 1, man_cpp);
    } else {
        for (size_t i = 0; i < size; ++i) {
            load_resource(nullptr, files[i], out_dir);
        }
    }
    if (man_h) {
        fclose(man_h);
    }
    if (man_cpp) {
        fclose(man_cpp);
    }
}

void init() {
    if (buffer == nullptr) {
        buffer = malloc(buffer_size);
    }
}

void deinit() {
    if (buffer != nullptr) {
        free(buffer);
        buffer = nullptr;
    }
}

struct binery_generator {
    char header_f[25600]{'\0'};
    size_t header_o{0};

    char source_f[25600]{'\0'};
    size_t source_o{0};
};

struct literal_generator {
    char header_f[25600]{'\0'};
    size_t header_o{0};

    char source_f[409600]{'\0'};
    size_t source_o{0};
};

struct resource_info {
    size_t offset{0};
    size_t size{0};
};

resource_info write(FILE *out, FILE *in) {

    resource_info info;
    info.offset = ftell(out);

    fseek(in, 0, SEEK_END);
    info.size = ftell(in);
    rewind(in);

    char buffer[2048];
    size_t wb = 0;

//    printf("start: %lu, size: %lu\n", info.offset, info.size);
    while (wb < info.size) {
        size_t rs = fread(buffer, 1, sizeof(buffer), in);
        wb += fwrite(buffer, 1, rs, out);
    }
    return info;
}

size_t write(char *buffer, const char *name, FILE *in) {

    const size_t buffer_size{16384};
    size_t buffer_offest{0};

    int c;
    buffer_offest +=
        snprintf(&buffer[buffer_offest], buffer_size - buffer_offest,
                 "const char *get_%s () { return \"", name);
    while (in && !feof(in)) {
        c = fgetc(in);
        if (c == '\n') {
            buffer_offest += snprintf(&buffer[buffer_offest],
                                      buffer_size - buffer_offest, "\\n");
        }
        if (isprint(c) && c != EOF) {
            buffer_offest += snprintf(&buffer[buffer_offest],
                                      buffer_size - buffer_offest, "%c", c);
        }
    }

    buffer_offest += snprintf(&buffer[buffer_offest],
                              buffer_size - buffer_offest, "\"; }\n");
    return buffer_offest;
}

void generate_resource_manager(const char **files, size_t size,
                               const char *binery_resource_file,
                               const char *out_dir) {

    void *local_buffer = buffer;

    binery_generator *bin = new (local_buffer) binery_generator();
    local_buffer = ptr_add(local_buffer, sizeof(binery_generator));

    literal_generator *lit = new (local_buffer) literal_generator();
    local_buffer = ptr_add(local_buffer, sizeof(literal_generator));

    char *out_buffer = (char *)local_buffer;

    char tmp_buffer[2048];
    sprintf(tmp_buffer, "%s/%s", out_dir, binery_resource_file);
    FILE *_out = fopen(tmp_buffer, "wb");
    if (_out) {
        for (size_t i = 0; i < size; ++i) {
            FILE *_in = fopen(files[i], "rb");
            if (_in) {
                char name[256];
                Storage type = get_storage_type(files[i]);
                get_resource_name_lower(name, 256, files[i]);
                if (type == BINERY) {
                    resource_info info = write(_out, _in);
                    fclose(_in);

                    bin->header_o +=
                        sprintf(&bin->header_f[bin->header_o],
                                manager_header_binery_function, name);

                    bin->source_o += sprintf(&bin->source_f[bin->source_o],
                                             manager_source_binery_function,
                                             name, info.offset, info.size);
                } else {

                    lit->header_o +=
                        sprintf(&lit->header_f[lit->header_o],
                                manager_header_literal_function, name);
                    lit->source_o +=
                        write(&lit->source_f[lit->source_o], name, _in);
                }
            }
        }
        fclose(_out);
    }
    size_t wb;

	wb = sprintf(out_buffer, manager_header, bin->header_f, lit->header_f);
	sprintf(tmp_buffer, "%s/resourcemanager.h", out_dir);
	if (crc32_fast(out_buffer, wb) != file_hash(tmp_buffer)) {
		_out = fopen(tmp_buffer, "wb");
		fwrite(out_buffer, 1, wb, _out);
		fclose(_out);
	}

    sprintf(tmp_buffer, "%s/resourcemanager.cpp", out_dir);
    wb = sprintf(out_buffer, manager_source, binery_resource_file,
                 bin->source_f, lit->source_f);

	if (crc32_fast(out_buffer, wb) != file_hash(tmp_buffer)) {
		_out = fopen(tmp_buffer, "wb");
		fwrite(out_buffer, 1, wb, _out);
		fclose(_out);
	}
}
