// Copyright (c) 2019 Roland Bernard
 

#include "vec2.h"

vec2_t vec2(scalar_t x, scalar_t y) {
	vec2_t ret = {
		.xy = { x, y }
	};
	return ret;
}

vec2_t vec2_scale(scalar_t a, vec2_t v) {
	vec2_t ret = {
		.xy = { v.xy[0]*a, v.xy[1]*a }
	};
	return ret;
}

vec2_t vec2_add(vec2_t v, vec2_t u) {
	vec2_t ret = {
		.xy = { v.xy[0]+u.xy[0], v.xy[1]+u.xy[1] }
	};
	return ret;
}

vec2_t vec2_reflect(vec2_t v, vec2_t n) {
	vec2_t nnorm = vec2_norm(n);
	return vec2_add(v, vec2_scale(-2*vec2_dot(v, nnorm), nnorm));
}

vec2_t vec2_norm(vec2_t v) {
	return vec2_scale(1 / vec2_mag(v), v);
}

scalar_t vec2_mag(vec2_t v) {
	return sqrtf(sqr(v.xy[0]) + sqr(v.xy[1]));
}

scalar_t vec2_dot(vec2_t v, vec2_t u) {
	return v.xy[0]*u.xy[0] + v.xy[1]*u.xy[1];
}

