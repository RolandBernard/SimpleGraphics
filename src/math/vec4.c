
#include "vec4.h"

vec4_t vec4(scalar_t x, scalar_t y, scalar_t z, scalar_t w) {
	vec4_t ret = {
		.xyzw = { x, y, z, w }
	};
	return ret;
}

vec4_t vec4_scale(scalar_t s, vec4_t v) {
	vec4_t ret = {
		.xyzw = { v.xyzw[0]*s, v.xyzw[1]*s, v.xyzw[2]*s, v.xyzw[3]*s }
	};
	return ret;
}

vec4_t vec4_add(vec4_t v, vec4_t u) {
	vec4_t ret = {
		.xyzw = { v.xyzw[0]+u.xyzw[0], v.xyzw[1]+u.xyzw[1], v.xyzw[2]+u.xyzw[2], v.xyzw[3]+u.xyzw[3] }
	};
	return ret;
}

vec4_t vec4_reflect(vec4_t v, vec4_t n) {
	vec4_t nnorm = vec4_norm(n);
	return vec4_add(v, vec4_scale(-2*vec4_dot(v, nnorm), nnorm));
}

vec4_t vec4_norm(vec4_t v) {
	return vec4_scale(1 / vec4_mag(v), v);
}

scalar_t vec4_mag(vec4_t v) {
	return sqrtf(sqr(v.xyzw[0]) + sqr(v.xyzw[1]) + sqr(v.xyzw[2]) + sqr(v.xyzw[3]));
}

scalar_t vec4_dot(vec4_t v, vec4_t u) {
	return v.xyzw[0]*u.xyzw[0] + v.xyzw[1]*u.xyzw[1] + v.xyzw[2]*u.xyzw[2] + v.xyzw[3]*u.xyzw[3];
}

