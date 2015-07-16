#ifndef RES_TEXT_FRAG_H
#define RES_TEXT_FRAG_H
struct res_text_frag { char buffer[273]; long int size; };
constexpr res_text_frag get_res_text_frag();
#endif
