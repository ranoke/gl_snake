#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "gl_math.h"
#include <stdint.h>

typedef struct r_object_t
{
	uint32_t count;
	uint32_t ibo_offset;
}r_object_t;

typedef struct shader_t
{
	uint32_t program;
}shader_t;

typedef struct renderer_t
{
	void(* init)(uint32_t, uint32_t);
	r_object_t(* load)(float*, uint32_t, uint32_t*, uint32_t);
	void(* draw)(r_object_t);
	void(* draw_pc)(r_object_t, vec3_t, vec3_t);
	void(* draw_tc)(r_object_t, mat4_t*, vec3_t);
	shader_t(* shader_construct)(const char*, const char*);
	void(* shader_set)(shader_t shader);
	void(* upload_projection)(mat4_t projection);
	void(* set_view)(mat4_t* view);

	
	uint32_t r_vao;
	uint32_t r_vbo;
	uint32_t r_ibo;
	uint32_t max_vertex_size;
	uint32_t max_index_size;
	mat4_t projection;


	shader_t shader;
}renderer_t;



extern renderer_t* renderer_construct();

extern renderer_t* renderer_ins;

#endif // __RENDERER_H__