#include "game.h"

#include <stdlib.h>
#include <string.h>

#define renderer renderer_ins

///////////
// forvard declarations
///////

g_object_t* g_object_ctor(vec3_t position, vec3_t scale, vec3_t color);
void g_object_render(g_object_t* g_obj);
void g_object_update(g_object_t* g_obj, vec3_t new_pos, vec3_t new_scale);
void g_object_destroy(g_object_t* g_obj);

snake_t snake_init();
void snake_move(snake_t* snake, vec3_t vec);
void snake_render(snake_t* snake);
void snake_addtail(snake_t* snake);

//////////
//  data
/////
float full_quad_vertices[] = {
   -1.0f,   1.0f,  0.0f,
   -1.0f,  -1.0f,  0.0f,  
    1.0f,  -1.0f,  0.0f,  
    1.0f,  +1.0f,  0.0f   
};

uint32_t full_quad_indices[] = {
    0, 1, 3,
    1, 2, 3
};



///////////
// implementations
/////

g_object_t* g_object_ctor(vec3_t position, vec3_t scale, vec3_t color)
{
	g_object_t* g_obj = (g_object_t*)malloc(sizeof(g_object_t));
	g_obj->color = color;
	g_obj->transform = mat4_identity();
	g_object_update(g_obj, position, scale);

	g_obj->r_object = renderer->load(full_quad_vertices, sizeof(full_quad_vertices),
						full_quad_indices, sizeof(full_quad_indices));
	return g_obj;
}

inline
void g_object_render(g_object_t* g_obj)
{
	renderer->draw_tc(g_obj->r_object, &g_obj->transform, g_obj->color);
}


void g_object_update(g_object_t* g_obj, vec3_t new_pos, vec3_t new_scale)
{
	g_obj->transform = mat4_translate(g_obj->transform, new_pos);
	g_obj->transform = mat4_scale(g_obj->transform, new_scale);
}

inline
void g_object_destroy(g_object_t* g_obj)
{
	free(g_obj);
}

inline
snake_t snake_init()
{
	snake_t snake = {0};
	snake.body[0] = *(g_object_t*)g_object_ctor(vec3_construct(0.f,0,0), vec3_construct(1,1,1), vec3_construct(0,0,0));
	snake.body[1] = *(g_object_t*)g_object_ctor(vec3_construct(2.f,2.f,0), vec3_construct(1,1,1), vec3_construct(0,0,0));
	snake.body[2] = *(g_object_t*)g_object_ctor(vec3_construct(4.f,4.f,0), vec3_construct(1,1,1), vec3_construct(0,0,0));
	snake.size = 3;
	
	return snake;
}

void snake_move(snake_t* snake, vec3_t vec)
{
	mat4_t prev_pos = snake->body[0].transform;
	g_object_update(&snake->body[0], vec, vec3_construct(1,1,1));
	for(int i = 1; i < snake->size; i++)
	{
		mat4_t temp = snake->body[i].transform;
		snake->body[i].transform = prev_pos;
		prev_pos = temp;
	}


}

void snake_render(snake_t* snake)
{
	uint32_t s = snake->size;
	for(int i = 0; i < s; i++)
	{
		g_object_render(&snake->body[i]);
	}
}

void snake_addtail(snake_t* snake)
{
	uint32_t s = snake->size;
	snake->body[s] = *(g_object_t*)g_object_ctor(vec3_construct(0,0,0), vec3_construct(1,1,1), vec3_construct(0,0,0));

	snake->size++;
}
