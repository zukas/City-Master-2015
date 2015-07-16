#include "resources.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctype.h>
#include "crc32.h"


enum ResType
{
	IMAGE,
	TEXT
};

ResType read_mime_type(const char *filename)
{
	ResType t = TEXT;
	char file_name[32] { '\0' };
	size_t length = strlen(filename);
	for(int i = length - 1; i >= 0; --i)
	{
		if(filename[i] == '.')
		{
			size_t k = 0;
			for(size_t j = i + 1; j < length; ++j, ++k)
			{
				char c = filename[j];
				if(isalnum(c) == 0)
				{
					break;
				}
				file_name[k] = tolower(c);
			}
			break;
		}
	}

	const char *array[]
	{
		"jpg",
		"jpeg",
		"png",
		"tga",
		"dds"
	};

	for(size_t i = 0; i < sizeof(array) / sizeof(const char*) && t == TEXT; ++i)
	{
		if(strcmp(array[i], file_name) == 0)
		{
			t = IMAGE;
		}
	}
	return t;
}

void get_filename(char *buffer, size_t size, const char *file, char delimeter = '/')
{
	size_t length = strlen(file);
	for(int i = length - 1; i >= -1; --i)
	{
		if(i == -1 || file[i] == delimeter)
		{
			size_t k = 0;
			for(size_t j = i + 1; j < length && k < size; ++j, ++k)
			{
				buffer[k] = file[j];
			}
			break;
		}
	}
}

void get_basename(char *buffer, size_t size, const char *file, char delimeter = '/')
{
	size_t length = strlen(file);
	for(int i = length - 1; i >= -1; --i)
	{
		if(i == -1 || file[i] == delimeter)
		{
			size_t k = 0;
			for(size_t j = i + 1; j < length && k < size; ++j, ++k)
			{
				if(file[i] == '.')
				{
					break;
				}
				buffer[k] = file[j];
			}
			break;
		}
	}
}

void get_path(char *buffer, size_t size, const char *file, char delimeter = '/')
{
	const char * end = strrchr(file, delimeter);
	if(end)
	{
		for(size_t i = 0; i < size && &file[i] != end; ++i)
		{
			buffer[i] = file[i];
		}
	}
}

uint32_t file_hash(const char *filename)
{
	uint32_t res { 0 };
	FILE *file = fopen64(filename, "rb");
	if(file)
	{
		fseeko64(file, 0, SEEK_END);
		long size = ftello64(file);
		rewind(file);
		char *tmp = (char*)malloc(size * sizeof(char));
		fread(tmp, 1, size, file);
		fclose(file);
		res = crc32_fast(tmp, size, 0);
		free(tmp);
	}

	return res;
}


void load_resource(char *out_filename, const char *file, const char *out_dir)
{
	ResType  type = read_mime_type(file);
	char file_name[128] { 'r', 'e', 's', '_' };


	get_filename(&file_name[4],124,file);
	for(size_t i = 0; i < strlen(file_name); ++i)
	{
		char &c = file_name[i];
		c = tolower(c);
		if(isalnum(c) == 0)
		{
			c = '_';
		}
	}


	char * name = (char*)malloc(sizeof(char) * strlen(file_name));
	strcpy(name, file_name);
	FILE *tmp = fopen64(file, "rb");
	if(tmp)
	{
		fseeko64(tmp, 0, SEEK_END);
		long size = ftello64(tmp);
		rewind(tmp);
		long buffer_size = (size << 2) * sizeof(int);
		long buffer_offest = 0;
		char *buffer = (char*)malloc(buffer_size);

		buffer_offest += snprintf(&buffer[buffer_offest], buffer_size - buffer_offest, "#include \"%s.h\"\nconstexpr %s get_%s() { return %s{{",name, name, name, name);

		int c = fgetc (tmp);
		buffer_offest += snprintf(&buffer[buffer_offest], buffer_size - buffer_offest, "%d", c);
		while(tmp && !feof(tmp))
		{
			c = fgetc (tmp);
			if(c != EOF)
			{
				buffer_offest += snprintf(&buffer[buffer_offest], buffer_size - buffer_offest, ",%d", c);
			}
		}

		buffer_offest += snprintf(&buffer[buffer_offest], buffer_size - buffer_offest, "},%ld}; }\n", size);

		fclose(tmp);

		char full_file_path[512] { '\0' };

		snprintf(full_file_path, 512, "%s/%s.cpp", out_dir, name);
        snprintf(out_filename, 512, "%s/%s.cpp", out_dir, name);
        size_t original_hash = file_hash(full_file_path);
		size_t generated_hash = crc32_fast(buffer, buffer_offest, 0);

//        printf("out: %s, original hash: %lu, new hash: %lu\n", full_file_path, original_hash, generated_hash);


		if(original_hash != generated_hash)
		{
//            printf("Generating sources for %s\n", file);
			tmp = fopen64(full_file_path, "wb");
			if(tmp)
			{
                printf("Updating %s\n", full_file_path);
				fwrite(buffer, 1, buffer_offest, tmp);
				fclose(tmp);
			}

//			snprintf(full_file_path, 512, "%s/%s.h", out_dir, name);
			tmp = fopen64(full_file_path, "wb");
			if(tmp)
			{
                printf("Updating %s\n", full_file_path);
				char __tmp[256] { '\0' };
				strcpy(__tmp, name);

				size_t i;
				for(i = 0; i < strlen(__tmp); ++i)
				{
					__tmp[i] = toupper(__tmp[i]);
				}
				__tmp[i] = '_';
				__tmp[i+1] = 'H';



				fprintf(tmp,"#ifndef %s\n#define %s\n", __tmp, __tmp);


				if(type == IMAGE)
				{
					fprintf(tmp, "struct %s { unsigned char buffer[%ld]; long int size; };\n", name, size);
				}
				else
				{
					fprintf(tmp, "struct %s { char buffer[%ld]; long int size; };\n", name, size);
				}

				fprintf(tmp, "constexpr %s get_%s();\n#endif\n", name, name);
				fclose(tmp);
			}
		}

		free(buffer);
		free(name);
	}
}


void generate_resources(filesname *out_files, const char **files, size_t size, const char *out_dir)
{
	for(size_t i = 0; i < size; ++i)
	{
		load_resource(out_files[i].name, files[i], out_dir);
//		printf("%s - %s\n", files[i], out_files[i].name);
	}
}
