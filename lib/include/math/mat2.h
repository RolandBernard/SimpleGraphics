// Copyright (c) 2019 Roland Bernard
 
#ifndef __MAT2_H__
#define __MAT2_H__

#include "scalar.h"
#include "vec2.h"

typedef struct {
	scalar_t d[2][2];
} mat2_t;

vec2_t mat2_vec2_mul(mat2_t m, vec2_t v);

mat2_t mat2_scale(scalar_t s, mat2_t m);

mat2_t mat2_add(mat2_t m, mat2_t n);

mat2_t mat2_mul(mat2_t m, mat2_t n);

mat2_t mat2_adj(mat2_t m);

mat2_t mat2_trans(mat2_t m);

mat2_t mat2_inv(mat2_t m);

mat2_t mat2_com(mat2_t m);

scalar_t mat2_det(mat2_t m);

#endif
