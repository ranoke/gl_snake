#include "renderer.h"

#include <glad/glad.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>


renderer_t renderer = {0};


uint32_t data_vbo_offset = 0;
uint32_t data_ibo_offset = 0;

uint32_t r_index = 0;

//////////////
// forward declarations
//////////

renderer_t* renderer_construct();

void __init(uint32_t vertex_size, uint32_t index_size);
r_object_t __load(float* vertices, uint32_t vertex_size, uint32_t* indices, uint32_t index_size);
void __draw(r_object_t object);
void __draw_pc(r_object_t object, vec3_t position, vec3_t color);
void __draw_tc(r_object_t object, mat4_t* tranf, vec3_t color);
shader_t __shader_construct(const char* vertex_src, const char* fragment_src);
void __shader_set(shader_t shader);
void __upload_projection(mat4_t projection);
void __set_view(mat4_t* view);




//////////////
// implementations
//////////

renderer_t* renderer_construct()
{
	renderer.init = &__init;
	renderer.load = &__load;
	renderer.draw = &__draw;
	renderer.draw_pc = &__draw_pc;
	renderer.draw_tc = &__draw_tc;
	renderer.shader_construct = &__shader_construct;
	renderer.shader_set = &__shader_set;
	renderer.upload_projection = &__upload_projection;
	renderer.set_view = &__set_view;
	return &renderer;
}

void __init(uint32_t vertex_size, uint32_t index_size)
{
	glGenBuffers(1, &renderer.r_vao);
	glBindVertexArray(renderer.r_vao);

	glGenBuffers(1, &renderer.r_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, renderer.r_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertex_size, 0, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &renderer.r_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer.r_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_size, 0, GL_DYNAMIC_DRAW);

	renderer.max_vertex_size = vertex_size;
	renderer.max_index_size = index_size;
}

r_object_t __load(float* vertices, uint32_t vertex_size, uint32_t* indices, uint32_t index_size)
{
	uint32_t* l_indices = malloc(index_size);
	uint32_t val_offset = data_ibo_offset / sizeof(uint32_t);
	for(int i = 0; i < index_size/sizeof(uint32_t); i++)
	{
		l_indices[i] = indices[i] + r_index;
	}


	glBindBuffer(GL_ARRAY_BUFFER, renderer.r_vbo);
	glBufferSubData(GL_ARRAY_BUFFER, data_vbo_offset, vertex_size, vertices);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer.r_ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, data_ibo_offset, index_size, l_indices);

	uint32_t vbo_offset = data_vbo_offset;
	uint32_t ibo_offset = data_ibo_offset;

	data_vbo_offset += vertex_size;
	data_ibo_offset += index_size;
	if(data_vbo_offset >= renderer.max_vertex_size || 
		data_ibo_offset >= renderer.max_index_size)
	{
		assert(false && "BufferOverflow");
		r_object_t o = {0};
		return o;
	}
	r_object_t o = {index_size/sizeof(uint32_t), ibo_offset};
	r_index = l_indices[index_size/sizeof(uint32_t) - 1] + 1;
	free(l_indices);
	return o;
}

void __draw(r_object_t object)
{
	glUseProgram(renderer.shader.program);
	glBindVertexArray(renderer.r_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer.r_ibo);

	glDrawElements(GL_TRIANGLES, object.count, GL_UNSIGNED_INT, object.ibo_offset);
}

void __draw_pc(r_object_t object, vec3_t position, vec3_t color)
{
	glUseProgram((uint32_t)(renderer.shader.program));
	uint32_t color_loc = glGetUniformLocation(renderer.shader.program, "u_color");
	glUniform3f(color_loc, color.x, color.y, color.z);

	glBindVertexArray(renderer.r_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer.r_ibo);

	glDrawElements(GL_TRIANGLES, object.count, GL_UNSIGNED_INT, object.ibo_offset);
	glUseProgram(0);
}

void __draw_tc(r_object_t object, mat4_t* tranf, vec3_t color)
{
	glUseProgram((uint32_t)(renderer.shader.program));
	uint32_t color_loc = glGetUniformLocation(renderer.shader.program, "u_color");
	glUniform3f(color_loc, color.x, color.y, color.z);

	int loc = glGetUniformLocation(renderer.shader.program, "u_model");
    glUniformMatrix4fv(loc, 1, GL_FALSE, tranf);

	glBindVertexArray(renderer.r_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer.r_ibo);

	glDrawElements(GL_TRIANGLES, object.count, GL_UNSIGNED_INT, object.ibo_offset);
	glUseProgram(0);
}


shader_t __shader_construct(const char* vertex_src, const char* fragment_src)
{

    uint32_t vertex_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_id, 1, &vertex_src, NULL);
    glCompileShader(vertex_id);
    {
	 	int success;
	    char infoLog[512];
	    glGetShaderiv(vertex_id, GL_COMPILE_STATUS, &success);
	    if(!success)
	    {
	        glGetShaderInfoLog(vertex_id, 512, NULL, infoLog);
	        printf("Error: %s", infoLog);
	        assert(false && "Vertex Failed compile shader!");
	    }
	}

    uint32_t fragment_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_id, 1, &fragment_src, NULL);
    glCompileShader(fragment_id);

    {
	 	int success;
	    char infoLog[512];
	    glGetShaderiv(fragment_id, GL_COMPILE_STATUS, &success);
	    if(!success)
	    {
	        glGetShaderInfoLog(fragment_id, 512, NULL, infoLog);
	        printf("Error: %s", infoLog);
	        assert(false && "Fragment Failed compile shader!");
	    }
	}      
   
    uint32_t program = glCreateProgram();
    glAttachShader(program, vertex_id);
    glAttachShader(program, fragment_id);
    glLinkProgram(program);
    
    {
	 	int success;
	    char infoLog[512];
	    glGetProgramiv(program, GL_LINK_STATUS, &success);
	    if(!success)
	    {
	        glGetShaderInfoLog(fragment_id, 512, NULL, infoLog);
	        printf("Error: %s", infoLog);
	        assert(false && "Link Failed shader!");
	    }
	}  
    
    glDeleteShader(vertex_id);
    glDeleteShader(fragment_id);
    shader_t shader = {program};

    return shader;
}

void __shader_set(shader_t shader)
{
	renderer.shader = shader;
}

void __upload_projection(mat4_t projection)
{
	glUseProgram(renderer.shader.program);
	renderer.projection = projection;
	int loc = glGetUniformLocation(renderer.shader.program, "u_projection");
    glUniformMatrix4fv(loc, 1, GL_FALSE, &renderer.projection);
}

void __set_view(mat4_t* view)
{
	glUseProgram(renderer.shader.program);

	int loc = glGetUniformLocation(renderer.shader.program, "u_view");
    glUniformMatrix4fv(loc, 1, GL_FALSE, view);
}