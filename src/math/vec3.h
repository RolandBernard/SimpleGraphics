// Copyright (c) 2019 Roland Bernard
 
#ifndef __VEC3_H__
#define __VEC3_H__

#include "scalar.h"

typedef struct {
	scalar_t xyz[3];
} vec3_t;

vec3_t vec3(scalar_t x, scalar_t y, scalar_t z);

vec3_t vec3_scale(scalar_t s, vec3_t v);

vec3_t vec3_add(vec3_t v, vec3_t u);

vec3_t vec3_reflect(vec3_t v, vec3_t n);

vec3_t vec3_cross(vec3_t v, vec3_t u);

vec3_t vec3_norm(vec3_t v);

scalar_t vec3_mag(vec3_t v);

scalar_t vec3_dot(vec3_t v, vec3_t u);

#endif
