#ifndef RES_IO_DDS_H
#define RES_IO_DDS_H
struct res_io_dds { unsigned char buffer[699216]; long int size; };
constexpr res_io_dds get_res_io_dds();
#endif
