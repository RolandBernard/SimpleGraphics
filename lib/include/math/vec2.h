// Copyright (c) 2019 Roland Bernard
 
#ifndef __VEC2_H__
#define __VEC2_H__

#include "scalar.h"

typedef struct {
	scalar_t xy[2];
} vec2_t;

vec2_t vec2(scalar_t x, scalar_t y);

vec2_t vec2_scale(scalar_t a, vec2_t v);

vec2_t vec2_add(vec2_t v, vec2_t u);

vec2_t vec2_reflect(vec2_t v, vec2_t n);

vec2_t vec2_norm(vec2_t v);

scalar_t vec2_mag(vec2_t v);

scalar_t vec2_dot(vec2_t v, vec2_t u);

#endif
