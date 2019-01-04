#ifndef __MAT4_H__
#define __MAT4_H__

#include "scalar.h"
#include "vec4.h"

typedef struct {
	scalar_t d[4][4];
} mat4_t;

vec4_t mat4_vec4_mul(mat4_t m, vec4_t v);

mat4_t mat4_scale(scalar_t s, mat4_t m);

mat4_t mat4_add(mat4_t m, mat4_t n);

mat4_t mat4_mul(mat4_t m, mat4_t n);

mat4_t mat4_adj(mat4_t m);

mat4_t mat4_trans(mat4_t m);

mat4_t mat4_inv(mat4_t m);

mat4_t mat4_com(mat4_t m);

scalar_t mat4_det(mat4_t m);

#endif
