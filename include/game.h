#ifndef __GAME_H__
#define __GAME_H__

#include "gl_math.h"
#include "renderer.h"
#include <stdint.h>

typedef struct vertex_t{
	vec3_t position;
}vertex_t;

typedef struct g_object_t{
	vec3_t color;	// next frame color
	mat4_t transform; // could do it without, but for now let it be
	r_object_t r_object;
}g_object_t;

typedef struct snake_t{
	g_object_t body[200];
	uint32_t size;
}snake_t;

g_object_t* g_object_ctor(vec3_t position, vec3_t scale, vec3_t color);
void g_object_render(g_object_t* g_obj);
void g_object_update(g_object_t* g_obj, vec3_t new_pos, vec3_t new_scale);
void g_object_destroy(g_object_t* g_obj);

snake_t snake_init();
void snake_move(snake_t* snake, vec3_t vec);
void snake_render(snake_t* snake);
void snake_addtail(snake_t* snake);


#endif // __GAME_H__