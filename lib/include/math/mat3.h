// Copyright (c) 2019 Roland Bernard
 
#ifndef __MAT3_H__
#define __MAT3_H__

#include "scalar.h"
#include "vec3.h"

typedef struct {
	scalar_t d[3][3];
} mat3_t;

vec3_t mat3_vec3_mul(mat3_t m, vec3_t v);

mat3_t mat3_scale(scalar_t s, mat3_t m);

mat3_t mat3_add(mat3_t m, mat3_t n);

mat3_t mat3_mul(mat3_t m, mat3_t n);

mat3_t mat3_adj(mat3_t m);

mat3_t mat3_trans(mat3_t m);

mat3_t mat3_inv(mat3_t m);

mat3_t mat3_com(mat3_t m);

scalar_t mat3_det(mat3_t m);

#endif
