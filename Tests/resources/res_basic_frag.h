#ifndef RES_BASIC_FRAG_H
#define RES_BASIC_FRAG_H
struct res_basic_frag { char buffer[103]; long int size; };
constexpr res_basic_frag get_res_basic_frag();
#endif
