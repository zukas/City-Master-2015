#ifndef TEMPLATES_H
#define TEMPLATES_H

#define NL "\n"

constexpr const char *manager_header{
    "#ifndef RESOURCES_H" NL "#define RESOURCES_H" NL "#include <cstddef>" NL
    "typedef void* (*malloc_function)(size_t);" NL
    "void set_malloc_function(malloc_function);" NL "void init_resources();" NL
    "void deinit_resources();" NL "struct resource {" NL
    "unsigned char *buffer;" NL "size_t size;" NL "};" NL
    // GENERATED BINERY RESOURCE GETTERS
    "%s" NL
    // GENERATED LITERAL RESOURCE GETTERS
    "%s" NL "#endif //RESOURCES_H"};

constexpr const char *manager_header_binery_function{
	"resource get_%s (void*);" NL "size_t get_%s_size();" NL};

constexpr const char *manager_header_literal_function{
	"const char *get_%s ();" NL};

constexpr const char *manager_source{
    "#include \"resourcemanager.h\"" NL "#include <cstdlib>" NL
    "#include <cstdio>" NL "malloc_function __g__malloc { ::malloc };" NL
    "FILE *__g_resource_handle { nullptr };" NL
    "void set_malloc_function(malloc_function func) { __g__malloc = func; }" NL
    "void init_resources() {	if(__g_resource_handle == nullptr) "
    "__g_resource_handle = fopen(\"%s\", \"rb\"); }" NL
    "void deinit_resources() { if(__g_resource_handle) "
    "fclose(__g_resource_handle); __g_resource_handle = nullptr; }" NL
	"resource load_binery_resource(void *buffer, size_t start, size_t size) "
	"{" NL "resource res;" NL "if(__g_resource_handle) {" NL
	"fseek(__g_resource_handle, start, SEEK_SET);" NL
	"res.buffer = (unsigned char *)buffer;" NL
	"res.size = fread(res.buffer, 1, size, __g_resource_handle);" NL "}" NL
    "return res;" NL "}" NL
    // GENERATED BINERY RESOURCE GETTERS
    "%s" NL
    // GENERATED LITERAL RESOURCE GETTERS
    "%s" NL};

constexpr const char *manager_source_binery_function{
	"resource get_%s (void* ptr_) { return load_binery_resource(ptr_, %lu, "
	"%lu); }" NL "size_t get_%s_size () { return %lu; }" NL};

#endif // TEMPLATES_H
