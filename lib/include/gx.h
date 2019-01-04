#ifndef __GX_H__
#define __GX_H__

#include "math/math.h"

typedef struct {
	vec4_t pos;
	scalar_t data[];
} gx_attr_t;

typedef scalar_t gx_color_t[3];

typedef void (*vert_shader_t)(void* udata, int vid, gx_attr_t* out_attr);

typedef void (*frag_shader_t)(void* udata, gx_attr_t* in_attr, gx_color_t* out_color);

typedef struct {
	int width;
	int height;
	gx_color_t* cbuffer;
	scalar_t* dbuffer;
} gx_render_target_t;

void clear_color(gx_render_target_t* target, gx_color_t color);

void clear_depth(gx_render_target_t* target, scalar_t d);

void render(gx_render_target_t* target, void* udata, int attr_size, int num_tri, vert_shader_t vshader, frag_shader_t fshader);

extern scalar_t gx_near_plane;

#endif
