#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <stdlib.h>


typedef struct window_t{
    void(* init)(struct window_t*);
    void(* update)(struct window_t*);
    void(* shutdown)(struct window_t*);
    const char* title;
    u_int32_t width;
    u_int32_t height;

    void* window;
}window_t;

extern window_t* window_construct(const char*, u_int32_t, u_int32_t);

#endif // __WINDOW_H__