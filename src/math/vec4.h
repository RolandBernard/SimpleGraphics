// Copyright (c) 2019 Roland Bernard
 
#ifndef __VEC4_H__
#define __VEC4_H__

#include "scalar.h"

typedef struct {
	scalar_t xyzw[4];
} vec4_t;

vec4_t vec4(scalar_t x, scalar_t y, scalar_t z, scalar_t w);

vec4_t vec4_scale(scalar_t s, vec4_t v);

vec4_t vec4_add(vec4_t v, vec4_t u);

vec4_t vec4_reflect(vec4_t v, vec4_t n);

vec4_t vec4_norm(vec4_t v);

scalar_t vec4_mag(vec4_t v);

scalar_t vec4_dot(vec4_t v, vec4_t u);

#endif
