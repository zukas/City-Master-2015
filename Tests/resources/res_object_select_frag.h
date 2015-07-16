#ifndef RES_OBJECT_SELECT_FRAG_H
#define RES_OBJECT_SELECT_FRAG_H
struct res_object_select_frag { char buffer[95]; long int size; };
constexpr res_object_select_frag get_res_object_select_frag();
#endif
